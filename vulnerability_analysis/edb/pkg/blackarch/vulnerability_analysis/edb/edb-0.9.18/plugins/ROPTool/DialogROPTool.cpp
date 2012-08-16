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

#include "DialogROPTool.h"
#include "DebuggerCoreInterface.h"
#include "Debugger.h"
#include "MemoryRegions.h"
#include "ByteShiftArray.h"
#include "Util.h"

#include <QDebug>
#include <QHeaderView>
#include <QMessageBox>
#include <QSortFilterProxyModel>
#include <QStandardItemModel>
#include <QModelIndex>

#include "ui_dialogrop.h"

//------------------------------------------------------------------------------
// Name: DialogROPTool(QWidget *parent)
// Desc:
//------------------------------------------------------------------------------
DialogROPTool::DialogROPTool(QWidget *parent) : QDialog(parent), ui(new Ui::DialogROPTool) {
	ui->setupUi(this);
	ui->tableView->verticalHeader()->hide();
	ui->tableView->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);

	filter_model_ = new QSortFilterProxyModel(this);
	connect(ui->txtSearch, SIGNAL(textChanged(const QString &)), filter_model_, SLOT(setFilterFixedString(const QString &)));


	result_model_ = new QStandardItemModel(this);
	result_filter_ = new ResultFilterProxy(this);
	result_filter_->setSourceModel(result_model_);
	ui->listView->setModel(result_filter_);
}

//------------------------------------------------------------------------------
// Name: ~DialogROPTool()
// Desc:
//------------------------------------------------------------------------------
DialogROPTool::~DialogROPTool() {
	delete ui;
}

//------------------------------------------------------------------------------
// Name: on_listView_itemDoubleClicked(QListViewItem *item)
// Desc: follows the found item in the data view
//------------------------------------------------------------------------------
void DialogROPTool::on_listView_doubleClicked(const QModelIndex &index) {
	bool ok;
	const edb::address_t addr = index.data(Qt::UserRole).toULongLong(&ok);
	if(ok) {
		edb::v1::jump_to_address(addr);
	}
}

//------------------------------------------------------------------------------
// Name: showEvent(QShowEvent *)
// Desc:
//------------------------------------------------------------------------------
void DialogROPTool::showEvent(QShowEvent *) {
	filter_model_->setFilterKeyColumn(3);
	filter_model_->setSourceModel(&edb::v1::memory_regions());
	ui->tableView->setModel(filter_model_);
	ui->progressBar->setValue(0);

	result_filter_->set_mask_bit(0x01, ui->chkShowALU->isChecked());
	result_filter_->set_mask_bit(0x02, ui->chkShowStack->isChecked());
	result_filter_->set_mask_bit(0x04, ui->chkShowLogic->isChecked());
	result_filter_->set_mask_bit(0x08, ui->chkShowData->isChecked());
	result_filter_->set_mask_bit(0x10, ui->chkShowOther->isChecked());

	result_model_->clear();
}

//------------------------------------------------------------------------------
// Name: on_chkShowALU_stateChanged(int state)
// Desc:
//------------------------------------------------------------------------------
void DialogROPTool::on_chkShowALU_stateChanged(int state) {
	result_filter_->set_mask_bit(0x01, state);
}

//------------------------------------------------------------------------------
// Name: on_chkShowStack_stateChanged(int state)
// Desc:
//------------------------------------------------------------------------------
void DialogROPTool::on_chkShowStack_stateChanged(int state) {
	result_filter_->set_mask_bit(0x02, state);
}

//------------------------------------------------------------------------------
// Name: on_chkShowLogic_stateChanged(int state)
// Desc:
//------------------------------------------------------------------------------
void DialogROPTool::on_chkShowLogic_stateChanged(int state) {
	result_filter_->set_mask_bit(0x04, state);
}

//------------------------------------------------------------------------------
// Name: on_chkShowData_stateChanged(int state)
// Desc:
//------------------------------------------------------------------------------
void DialogROPTool::on_chkShowData_stateChanged(int state) {
	result_filter_->set_mask_bit(0x08, state);
}

//------------------------------------------------------------------------------
// Name: on_chkShowOther_stateChanged(int state)
// Desc:
//------------------------------------------------------------------------------
void DialogROPTool::on_chkShowOther_stateChanged(int state) {
	result_filter_->set_mask_bit(0x10, state);
}

//------------------------------------------------------------------------------
// Name: set_gadget_role(QStandardItem *item, const edb::Instruction &insn1)
// Desc:
//------------------------------------------------------------------------------
void DialogROPTool::set_gadget_role(QStandardItem *item, const edb::Instruction &insn1) {

	switch(insn1.type()) {
	case edb::Instruction::OP_ADD:
	case edb::Instruction::OP_ADC:
	case edb::Instruction::OP_SUB:
	case edb::Instruction::OP_SBB:
	case edb::Instruction::OP_IMUL:
	case edb::Instruction::OP_MUL:
	case edb::Instruction::OP_IDIV:
	case edb::Instruction::OP_DIV:
	case edb::Instruction::OP_INC:
	case edb::Instruction::OP_DEC:
	case edb::Instruction::OP_NEG:
	case edb::Instruction::OP_CMP:
	case edb::Instruction::OP_DAA:
	case edb::Instruction::OP_DAS:
	case edb::Instruction::OP_AAA:
	case edb::Instruction::OP_AAS:
	case edb::Instruction::OP_AAM:
	case edb::Instruction::OP_AAD:
		// ALU ops
		item->setData(0x01, Qt::UserRole + 1);
		break;
	case edb::Instruction::OP_PUSH:
	case edb::Instruction::OP_PUSHA:
	case edb::Instruction::OP_POP:
	case edb::Instruction::OP_POPA:
		// stack ops
		item->setData(0x02, Qt::UserRole + 1);
		break;
	case edb::Instruction::OP_AND:
	case edb::Instruction::OP_OR:
	case edb::Instruction::OP_XOR:
	case edb::Instruction::OP_NOT:
	case edb::Instruction::OP_SAR:
	case edb::Instruction::OP_SAL:
	case edb::Instruction::OP_SHR:
	case edb::Instruction::OP_SHL:
	case edb::Instruction::OP_SHRD:
	case edb::Instruction::OP_SHLD:
	case edb::Instruction::OP_ROR:
	case edb::Instruction::OP_ROL:
	case edb::Instruction::OP_RCR:
	case edb::Instruction::OP_RCL:
	case edb::Instruction::OP_BT:
	case edb::Instruction::OP_BTS:
	case edb::Instruction::OP_BTR:
	case edb::Instruction::OP_BTC:
	case edb::Instruction::OP_BSF:
	case edb::Instruction::OP_BSR:
		// logic ops
		item->setData(0x04, Qt::UserRole + 1);
		break;
	case edb::Instruction::OP_MOV:
	case edb::Instruction::OP_CMOVCC:
	case edb::Instruction::OP_XCHG:
	case edb::Instruction::OP_BSWAP:
	case edb::Instruction::OP_XADD:
	case edb::Instruction::OP_CMPXCHG:
	case edb::Instruction::OP_CWD:
	case edb::Instruction::OP_CDQ:
	case edb::Instruction::OP_CQO:
	case edb::Instruction::OP_CDQE:
	case edb::Instruction::OP_CBW:
	case edb::Instruction::OP_CWDE:
	case edb::Instruction::OP_MOVSX:
	case edb::Instruction::OP_MOVZX:
	case edb::Instruction::OP_MOVSXD:
	case edb::Instruction::OP_MOVBE:
	case edb::Instruction::OP_MOVS:
	case edb::Instruction::OP_CMPS:
	case edb::Instruction::OP_CMPSW:
	case edb::Instruction::OP_SCAS:
	case edb::Instruction::OP_LODS:
	case edb::Instruction::OP_STOS:
	case edb::Instruction::OP_CMPXCHG8B:
	case edb::Instruction::OP_CMPXCHG16B:
		// data ops
		item->setData(0x08, Qt::UserRole + 1);
		break;
	default:
		// other ops
		item->setData(0x10, Qt::UserRole + 1);
		break;

	}
}

//------------------------------------------------------------------------------
// Name: add_gadget(edb::address_t start_address, QList<edb::Instruction> instructions, size_t size)
// Desc:
//------------------------------------------------------------------------------
void DialogROPTool::add_gadget(edb::address_t start_address, QList<edb::Instruction> instructions, size_t size) {

	if(!instructions.isEmpty()) {
		const edb::Instruction insn1 = instructions.takeFirst();

		QString instruction_string = QString("%1").arg(QString::fromStdString(edisassm::to_string(insn1)));
		Q_FOREACH(const edb::Instruction &instruction, instructions) {
			instruction_string.append(QString("; %1").arg(QString::fromStdString(edisassm::to_string(instruction))));
		}

		if(!ui->checkUnique->isChecked() || !unique_results_.contains(instruction_string)) {
			unique_results_.insert(instruction_string);

			// found a gadget
			QStandardItem *const item = new QStandardItem(QString("%1: %2").arg(
					edb::v1::format_pointer(start_address - size + 1),
					instruction_string));

			item->setData(static_cast<qulonglong>(start_address - size + 1), Qt::UserRole);
			set_gadget_role(item, insn1);

			result_model_->insertRow(result_model_->rowCount(), item);
		}
	}
}

//------------------------------------------------------------------------------
// Name: do_find()
// Desc:
//------------------------------------------------------------------------------
void DialogROPTool::do_find() {

	const QItemSelectionModel *const selModel = ui->tableView->selectionModel();
	const QModelIndexList sel = selModel->selectedRows();

	if(sel.size() == 0) {
		QMessageBox::information(
			this,
			tr("No Region Selected"),
			tr("You must select a region which is to be scanned for gadgets."));
	} else {

		unique_results_.clear();

		Q_FOREACH(const QModelIndex &selected_item, sel) {

			const QModelIndex index = filter_model_->mapToSource(selected_item);
			const MemRegion *const region = reinterpret_cast<const MemRegion *>(index.internalPointer());

			edb::address_t start_address     = region->start;
			const edb::address_t end_address = region->end;
			const edb::address_t orig_start  = start_address;
			size_t size = 0;

			ByteShiftArray bsa(64);

			while(start_address < end_address) {

				// TODO: allow NOPs or effective nops in the streams

				// read in the next byte
				quint8 byte;
				if(edb::v1::debugger_core->read_bytes(start_address, &byte, 1)) {
					bsa << byte;

					size = qMin(size + 1, bsa.size());

					const quint8 *const p = bsa.data();
					edb::Instruction insn1(p, size, start_address - size + 1, std::nothrow);

					if(insn1.valid()) {
					
						if(insn1.type() == edb::Instruction::OP_INT && insn1.operand(0).general_type() == edb::Operand::TYPE_IMMEDIATE && (insn1.operand(0).immediate() & 0xff) == 0x80) {
							add_gadget(start_address, (QList<edb::Instruction>() << insn1), size);
						} else if(insn1.type() == edb::Instruction::OP_SYSENTER) {
							add_gadget(start_address, (QList<edb::Instruction>() << insn1), size);
						} else if(insn1.type() == edb::Instruction::OP_SYSCALL) {
							add_gadget(start_address, (QList<edb::Instruction>() << insn1), size);
						} else {
					
							edb::Instruction insn2(p + insn1.size(), size - insn1.size(), 0, std::nothrow);

							if(insn2.valid() && insn2.type() == edb::Instruction::OP_RET) {
								add_gadget(start_address, (QList<edb::Instruction>() << insn1 << insn2), size);
							} else if(insn2.valid() && insn2.type() == edb::Instruction::OP_POP) {

								edb::Instruction insn3(p + insn1.size() + insn2.size(), size - insn1.size() - insn2.size(), 0, std::nothrow);
								if(insn3.valid() && insn3.type() == edb::Instruction::OP_JMP) {

									if(insn2.operand_count() == 1 && insn2.operand(0).general_type() == edb::Operand::TYPE_REGISTER) {
										if(insn3.operand_count() == 1 && insn3.operand(0).general_type() == edb::Operand::TYPE_REGISTER) {
											if(insn2.operand(0).reg() == insn3.operand(0).reg()) {
												add_gadget(start_address, (QList<edb::Instruction>() << insn1 << insn2 << insn3), size);
											}
										}
									}
								}
							}
						}

						// TODO: catch things like "add rsp, 8; jmp [rsp - 8]" and similar, it's rare,
						// but could happen
					}
				}

				ui->progressBar->setValue(util::percentage(start_address - orig_start, region->size()));
				++start_address;
			}
		}
	}
}

//------------------------------------------------------------------------------
// Name: on_btnFind_clicked()
// Desc:
//------------------------------------------------------------------------------
void DialogROPTool::on_btnFind_clicked() {
	ui->btnFind->setEnabled(false);
	ui->progressBar->setValue(0);
	result_model_->clear();
	do_find();
	ui->progressBar->setValue(100);
	ui->btnFind->setEnabled(true);
}
