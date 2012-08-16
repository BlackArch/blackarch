/*
Copyright (C) 2006 - 2011 Evan Teran
                          eteran@alum.rit.edu

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "DialogAttach.h"
#include "Types.h"

#include <QMap>
#include <QHeaderView>
#include <QDir>
#include <QTextStream>

#include <cstring>

#include <windows.h>
#include <tlhelp32.h>

#include "ui_dialog_attach.h"

struct ProcessInfo {
	edb::pid_t pid;
	edb::uid_t uid;
	QString    user;
	QString    name;
};

namespace {
	typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
	LPFN_ISWOW64PROCESS fnIsWow64Process;
}

#ifdef _MSC_VER
#pragma comment(lib, "Advapi32.lib")
#endif

//------------------------------------------------------------------------------
// Name: DialogAttach(QWidget *parent) : QDialog(parent)
// Desc: constructor
//------------------------------------------------------------------------------
DialogAttach::DialogAttach(QWidget *parent) : QDialog(parent), ui(new Ui::DialogAttach) {
	ui->setupUi(this);

	// resolve the "IsWow64Process" function since it may or may not exist
	fnIsWow64Process = (LPFN_ISWOW64PROCESS) GetProcAddress(GetModuleHandle(TEXT("kernel32")), "IsWow64Process");
}

//------------------------------------------------------------------------------
// Name: ~DialogAttach()
// Desc:
//------------------------------------------------------------------------------
DialogAttach::~DialogAttach() {
	delete ui;
}

//------------------------------------------------------------------------------
// Name: on_filter_textChanged(const QString &text)
// Desc:
//------------------------------------------------------------------------------
void DialogAttach::on_filter_textChanged(const QString &text) {
	update_list(text);
}

namespace {
QString dumpToken(HANDLE hProcess) {
	QString ret;
	HANDLE hToken;
	if(!OpenProcessToken(hProcess, TOKEN_QUERY, &hToken))
		return ret;

	DWORD needed;
	TOKEN_OWNER *tokenOwner = NULL;
	GetTokenInformation(hToken, TokenOwner, NULL, 0, &needed);
	tokenOwner = static_cast<TOKEN_OWNER *>(malloc(needed));

	if(tokenOwner) {
		if(GetTokenInformation(hToken, TokenOwner, tokenOwner, needed, &needed)) {
			WCHAR user[MAX_PATH];
			WCHAR domain[MAX_PATH];
			SID_NAME_USE snu;
			DWORD user_sz = MAX_PATH;
			DWORD domain_sz = MAX_PATH;
			if(LookupAccountSid(NULL, tokenOwner->Owner, user, &user_sz, domain, &domain_sz, &snu) && snu == SidTypeUser) {
				ret = QString::fromWCharArray(user);
			}
		}
		free(tokenOwner);
	}

	CloseHandle(hToken);
	return ret;
}
}

//------------------------------------------------------------------------------
// Name: update_list(const QString &filter)
// Desc:
//------------------------------------------------------------------------------
void DialogAttach::update_list(const QString &filter) {

	QMap<edb::pid_t, ProcessInfo> procs;
	edb::uid_t myUID = 0;

	// FILL up procs
	HANDLE handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if(handle != INVALID_HANDLE_VALUE) {
		PROCESSENTRY32 lppe;

		std::memset(&lppe, 0, sizeof(lppe));
		lppe.dwSize = sizeof(lppe);

		if(Process32First(handle, &lppe)) {
			do {
				ProcessInfo pi;
				pi.pid = lppe.th32ProcessID;
				pi.uid = 0; // TODO
				pi.name = QString::fromWCharArray(lppe.szExeFile);

				HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, lppe.th32ProcessID);
				if(hProc != 0) {
					BOOL wow64 = FALSE;
					if(fnIsWow64Process && fnIsWow64Process(hProc, &wow64) && wow64) {
						pi.name += " *32";
					}

					pi.user = dumpToken(hProc);

					CloseHandle(hProc);
				}

				procs[pi.pid] = pi;

				std::memset(&lppe, 0, sizeof(lppe));
				lppe.dwSize = sizeof(lppe);
			} while(Process32Next(handle, &lppe));
		}

		CloseHandle(handle);
	}

	const bool filterUID = ui->filter_uid->isChecked();
	const QString lowerFilter = filter.toLower();
	ui->processes_table->setSortingEnabled(false);
	ui->processes_table->setRowCount(0);

	Q_FOREACH(const ProcessInfo &procInfo, procs) {

		const QString procName = procInfo.name;

		if(filter.isEmpty() || procName.toLower().contains(lowerFilter)) {
			if(!filterUID || procInfo.uid == myUID) {
				const int row = ui->processes_table->rowCount();
				ui->processes_table->insertRow(row);

				QTableWidgetItem *const itemPID = new QTableWidgetItem;
				itemPID->setData(Qt::DisplayRole, static_cast<quint32>(procInfo.pid));

				QTableWidgetItem *itemUID;
				if(!procInfo.user.isEmpty()) {
					itemUID = new QTableWidgetItem(procInfo.user);
				} else {
					itemUID = new QTableWidgetItem;
					itemUID->setData(Qt::DisplayRole, static_cast<quint32>(procInfo.uid));
				}

				ui->processes_table->setItem(row, 0, itemPID);
				ui->processes_table->setItem(row, 1, itemUID);
		        ui->processes_table->setItem(row, 2, new QTableWidgetItem(procInfo.name));
			}
		}
	}
	ui->processes_table->setSortingEnabled(true);
}

//------------------------------------------------------------------------------
// Name: showEvent(QShowEvent *event)
// Desc:
//------------------------------------------------------------------------------
void DialogAttach::showEvent(QShowEvent *event) {
	Q_UNUSED(event);
	update_list(ui->filter->text());
}

//------------------------------------------------------------------------------
// Name: on_filter_uid_clicked(bool checked)
// Desc:
//------------------------------------------------------------------------------
void DialogAttach::on_filter_uid_clicked(bool checked) {
	Q_UNUSED(checked);
	update_list(ui->filter->text());
}

//------------------------------------------------------------------------------
// Name: selected_pid(bool &ok) const
// Desc:
//------------------------------------------------------------------------------
edb::pid_t DialogAttach::selected_pid(bool &ok) const {
	const QList<QTableWidgetItem *> sel = ui->processes_table->selectedItems();
	if(sel.size() != 0) {
		ok = true;
		return sel.first()->text().toUInt();
	} else {
		ok = false;
		return edb::pid_t();
	}
}
