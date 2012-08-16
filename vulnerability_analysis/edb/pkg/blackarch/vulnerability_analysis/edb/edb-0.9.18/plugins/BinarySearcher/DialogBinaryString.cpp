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

#include "DialogBinaryString.h"
#include "DebuggerCoreInterface.h"
#include "MemoryRegions.h"
#include "Debugger.h"
#include "ByteShiftArray.h"
#include "Util.h"
#include "Debugger.h"

#include <QVector>
#include <QMessageBox>
#include <cstring>

#include "ui_dialogbinarystring.h"

//------------------------------------------------------------------------------
// Name: DialogBinaryString(QWidget *parent)
// Desc: constructor
//------------------------------------------------------------------------------
DialogBinaryString::DialogBinaryString(QWidget *parent) : QDialog(parent), ui(new Ui::DialogBinaryString) {
	ui->setupUi(this);
	ui->progressBar->setValue(0);
	ui->listWidget->clear();
}

//------------------------------------------------------------------------------
// Name: ~DialogBinaryString()
// Desc:
//------------------------------------------------------------------------------
DialogBinaryString::~DialogBinaryString() {
	delete ui;
}

//------------------------------------------------------------------------------
// Name: do_find()
// Desc:
//------------------------------------------------------------------------------
void DialogBinaryString::do_find() {

	const QByteArray b = ui->binaryString->value();
	ui->listWidget->clear();

	const int sz = b.size();
	if(sz != 0) {
		ByteShiftArray bsa(sz);

		edb::v1::memory_regions().sync();
		const QList<MemRegion> regions = edb::v1::memory_regions().regions();
		const edb::address_t page_size = edb::v1::debugger_core->page_size();

		int i = 0;
		Q_FOREACH(const MemRegion &region, regions) {

			bsa.clear();

			// a short circut for speading things up
			if(ui->chkSkipNoAccess->isChecked() && !region.accessible()) {
				ui->progressBar->setValue(util::percentage(++i, regions.size()));
				continue;
			}

			const edb::address_t size_in_pages = region.size() / page_size;
			try {
				QVector<quint8> pages(size_in_pages * page_size);
				const quint8 *const pages_end = &pages[0] + region.size();

				if(edb::v1::debugger_core->read_pages(region.start, &pages[0], size_in_pages)) {
					const quint8 *p = &pages[0];
					QString temp;
					while(p != pages_end) {
						// shift in the next byte
						bsa << *p;

						// compare values..
						if(std::memcmp(bsa.data(), b.constData(), sz) == 0) {
							const edb::address_t addr = (p - &pages[0] + region.start) - sz + 1;
							const edb::address_t align = 1 << (ui->cmbAlignment->currentIndex() + 1);

							if(!ui->chkAlignment->isChecked() || (addr % align) == 0) {
								ui->listWidget->addItem(edb::v1::format_pointer(addr));
							}
						}

						ui->progressBar->setValue(util::percentage(i, regions.size(), p - &pages[0], region.size()));

						++p;
					}
				}
				++i;
			} catch(const std::bad_alloc &) {
				QMessageBox::information(
					0,
					tr("Memroy Allocation Error"),
					tr("Unable to satisfy memory allocation request for requested region."));
			}
		}
	}
}

//------------------------------------------------------------------------------
// Name: on_btnFind_clicked()
// Desc: find button event handler
//------------------------------------------------------------------------------
void DialogBinaryString::on_btnFind_clicked() {

	ui->btnFind->setEnabled(false);
	ui->progressBar->setValue(0);
	do_find();
	ui->progressBar->setValue(100);
	ui->btnFind->setEnabled(true);
}

//------------------------------------------------------------------------------
// Name: on_listWidget_itemDoubleClicked(QListWidgetItem *item)
// Desc: follows the found item in the data view
//------------------------------------------------------------------------------
void DialogBinaryString::on_listWidget_itemDoubleClicked(QListWidgetItem *item) {
	bool ok;
	const edb::address_t addr = edb::v1::string_to_address(item->text(), ok);
	if(ok) {
		edb::v1::dump_data(addr, false);
	}
}
