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

#include "DialogOpenFiles.h"
#include "DebuggerCoreInterface.h"
#include "Debugger.h"

#include <QtDebug>
#include <QHeaderView>
#include <QTextStream>
#include <QDir>
#include <QHostAddress>

#include <arpa/inet.h>

#include "ui_dialogopenfiles.h"

// this file is generally very linux specific
#ifndef Q_OS_LINUX
#error "Unsupported Platform"
#endif


namespace {
//------------------------------------------------------------------------------
// Name: tcp_socket_prcoessor(QString &symlink, int sock, const QStringList &lst)
// Desc:
//------------------------------------------------------------------------------
bool tcp_socket_prcoessor(QString &symlink, int sock, const QStringList &lst) {
	if(lst.size() >= 13) {

		bool ok;
		const quint32 local_address = ntohl(lst[1].toUInt(&ok, 16));
		if(ok) {
			const quint16 local_port = lst[2].toUInt(&ok, 16);
			if(ok) {
				const quint32 remote_address = ntohl(lst[3].toUInt(&ok, 16));
				if(ok) {
					const quint16 remote_port = lst[4].toUInt(&ok, 16);
					if(ok) {
						const quint8 state = lst[5].toUInt(&ok, 16);
						Q_UNUSED(state);
						if(ok) {
							const int inode = lst[13].toUInt(&ok, 10);
							if(ok) {
								if(inode == sock) {
									symlink = QString("TCP: %1:%2 -> %3:%4")
										.arg(QHostAddress(local_address).toString())
										.arg(local_port)
										.arg(QHostAddress(remote_address).toString())
										.arg(remote_port);
										return true;
								}
							}
						}
					}
				}
			}
		}
	}
	return false;
}

//------------------------------------------------------------------------------
// Name: udp_socket_processor(QString &symlink, int sock, const QStringList &lst)
// Desc:
//------------------------------------------------------------------------------
bool udp_socket_processor(QString &symlink, int sock, const QStringList &lst) {
	if(lst.size() >= 13) {

		bool ok;
		const quint32 local_address = ntohl(lst[1].toUInt(&ok, 16));
		if(ok) {
			const quint16 local_port = lst[2].toUInt(&ok, 16);
			if(ok) {
				const quint32 remote_address = ntohl(lst[3].toUInt(&ok, 16));
				if(ok) {
					const quint16 remote_port = lst[4].toUInt(&ok, 16);
					if(ok) {
						const quint8 state = lst[5].toUInt(&ok, 16);
						Q_UNUSED(state);
						if(ok) {
							const int inode = lst[13].toUInt(&ok, 10);
							if(ok) {
								if(inode == sock) {
									symlink = QString("UDP: %1:%2 -> %3:%4")
										.arg(QHostAddress(local_address).toString())
										.arg(local_port)
										.arg(QHostAddress(remote_address).toString())
										.arg(remote_port);
										return true;
								}
							}
						}
					}
				}
			}
		}
	}
	return false;
}

//------------------------------------------------------------------------------
// Name: unix_socket_processor(QString &symlink, int sock, const QStringList &lst)
// Desc:
//------------------------------------------------------------------------------
bool unix_socket_processor(QString &symlink, int sock, const QStringList &lst) {
	if(lst.size() >= 6) {
		bool ok;
		const int inode = lst[6].toUInt(&ok, 10);
		if(ok) {
			if(inode == sock) {
				symlink = QString("UNIX [%1]").arg(lst[0]);
				return true;
			}
		}
	}
	return false;
}

//------------------------------------------------------------------------------
// Name: process_socket_file(const QString &filename, QString &symlink, int sock, F fp)
// Desc:
//------------------------------------------------------------------------------
template <class F>
QString process_socket_file(const QString &filename, QString &symlink, int sock, F fp) {
	QFile net(filename);
	net.open(QIODevice::ReadOnly | QIODevice::Text);
	if(net.isOpen()) {
		QTextStream in(&net);
		QString line;

		// ditch first line, it is just table headings
		line = in.readLine();

		// read in the first line we care about
		line = in.readLine();

		// a null string means end of file (but not an empty string!)
		while(!line.isNull()) {

			QString lline(line);
			const QStringList lst = lline.replace(":", " ").split(" ", QString::SkipEmptyParts);

			if(fp(symlink, sock, lst)) {
				break;
			}

			line = in.readLine();
		}
	}
	return symlink;
}

}

//------------------------------------------------------------------------------
// Name: DialogOpenFiles(QWidget *parent)
// Desc:
//------------------------------------------------------------------------------
DialogOpenFiles::DialogOpenFiles(QWidget *parent) : QDialog(parent), ui(new Ui::DialogOpenFiles) {
	ui->setupUi(this);
	ui->tableWidget->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
}

//------------------------------------------------------------------------------
// Name: ~DialogOpenFiles()
// Desc:
//------------------------------------------------------------------------------
DialogOpenFiles::~DialogOpenFiles() {
	delete ui;
}

//------------------------------------------------------------------------------
// Name: showEvent(QShowEvent *)
// Desc:
//------------------------------------------------------------------------------
void DialogOpenFiles::showEvent(QShowEvent *) {
	on_btnRefresh_clicked();
}

//------------------------------------------------------------------------------
// Name: file_type(const QString &filename)
// Desc:
//------------------------------------------------------------------------------
QString DialogOpenFiles::file_type(const QString &filename) {
	QFileInfo info(filename);
	QString basename(info.completeBaseName());

	if(basename.startsWith("socket:")) {
		return tr("Socket");
	}

	if(basename.startsWith("pipe:")) {
		return tr("Pipe");
	}

	return tr("File");
}

//------------------------------------------------------------------------------
// Name: process_socket_tcp(QString &symlink)
// Desc:
//------------------------------------------------------------------------------
QString DialogOpenFiles::process_socket_tcp(QString &symlink) {
	const QString socket_info(symlink.mid(symlink.indexOf("socket:[")));
	const int socket_number = socket_info.mid(8).remove("]").toUInt();

	return process_socket_file("/proc/net/tcp", symlink, socket_number, tcp_socket_prcoessor);
}

//------------------------------------------------------------------------------
// Name: process_socket_unix(QString &symlink)
// Desc:
//------------------------------------------------------------------------------
QString DialogOpenFiles::process_socket_unix(QString &symlink) {
	const QString socket_info(symlink.mid(symlink.indexOf("socket:[")));
	const int socket_number = socket_info.mid(8).remove("]").toUInt();

	return process_socket_file("/proc/net/unix", symlink, socket_number, unix_socket_processor);
}

//------------------------------------------------------------------------------
// Name: process_socket_udp(QString &symlink)
// Desc:
//------------------------------------------------------------------------------
QString DialogOpenFiles::process_socket_udp(QString &symlink) {
	const QString socket_info(symlink.mid(symlink.indexOf("socket:[")));
	const int socket_number = socket_info.mid(8).remove("]").toUInt();

	return process_socket_file("/proc/net/udp", symlink, socket_number, udp_socket_processor);
}

//------------------------------------------------------------------------------
// Name: do_find()
// Desc:
//------------------------------------------------------------------------------
void DialogOpenFiles::do_find() {
	ui->tableWidget->setSortingEnabled(false);
	ui->tableWidget->setRowCount(0);

	const edb::pid_t pid = edb::v1::debugger_core->pid();
	if(pid != -1) {
		QDir dir(QString("/proc/%1/fd/").arg(pid));
		const QFileInfoList entries = dir.entryInfoList(QStringList() << "[0-9]*");
		Q_FOREACH(const QFileInfo &info, entries) {
			if(info.isSymLink()) {
				QString symlink(info.symLinkTarget());
				const QString type(file_type(symlink));

				if(type == tr("Socket")) {
					symlink = process_socket_tcp(symlink);
					symlink = process_socket_udp(symlink);
					symlink = process_socket_unix(symlink);
				}

				if(type == tr("Pipe")) {
					symlink = tr("FIFO");
				}

				const int row = ui->tableWidget->rowCount();
				ui->tableWidget->insertRow(row);


				QTableWidgetItem *const itemFD = new QTableWidgetItem;
				itemFD->setData(Qt::DisplayRole, info.fileName().toUInt());

				ui->tableWidget->setItem(row, 0, itemFD);
	        	ui->tableWidget->setItem(row, 1, new QTableWidgetItem(type));
				ui->tableWidget->setItem(row, 2, new QTableWidgetItem(symlink));
			}
		}
	}

	ui->tableWidget->setSortingEnabled(true);
}

//------------------------------------------------------------------------------
// Name: on_btnRefresh_clicked()
// Desc:
//------------------------------------------------------------------------------
void DialogOpenFiles::on_btnRefresh_clicked() {
	ui->btnRefresh->setEnabled(false);
	do_find();
	ui->btnRefresh->setEnabled(true);
}
