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

#include "DialogOpcodes.h"
#include "DebuggerCoreInterface.h"
#include "Debugger.h"
#include "MemoryRegions.h"
#include "ByteShiftArray.h"
#include "Util.h"

#include <QHeaderView>
#include <QMessageBox>
#include <QSortFilterProxyModel>
#include <QListWidgetItem>
#include <QDebug>

#include "ui_dialogopcodes.h"

namespace {
#if defined(EDB_X86)
	const edb::Operand::Register STACK_REG = edb::Operand::REG_ESP;
#elif defined(EDB_X86_64)
	const edb::Operand::Register STACK_REG = edb::Operand::REG_RSP;
#endif
}

//------------------------------------------------------------------------------
// Name: DialogOpcodes(QWidget *parent)
// Desc:
//------------------------------------------------------------------------------
DialogOpcodes::DialogOpcodes(QWidget *parent) : QDialog(parent), ui(new Ui::DialogOpcodes) {
	ui->setupUi(this);
	ui->tableView->verticalHeader()->hide();
	ui->tableView->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);

	filter_model_ = new QSortFilterProxyModel(this);
	connect(ui->txtSearch, SIGNAL(textChanged(const QString &)), filter_model_, SLOT(setFilterFixedString(const QString &)));

#if defined(EDB_X86)
	ui->comboBox->addItem("EAX -> EIP", 1);
	ui->comboBox->addItem("EBX -> EIP", 2);
	ui->comboBox->addItem("ECX -> EIP", 3);
	ui->comboBox->addItem("EDX -> EIP", 4);
	ui->comboBox->addItem("EBP -> EIP", 5);
	ui->comboBox->addItem("ESP -> EIP", 6);
	ui->comboBox->addItem("ESI -> EIP", 7);
	ui->comboBox->addItem("EDI -> EIP", 8);
	ui->comboBox->addItem("ANY REGISTER -> EIP", 17);
	ui->comboBox->addItem("[ESP] -> EIP", 18);
	ui->comboBox->addItem("[ESP + 4] -> EIP", 19);
	ui->comboBox->addItem("[ESP + 8] -> EIP", 20);
	ui->comboBox->addItem("[ESP - 4] -> EIP", 21);
#elif defined(EDB_X86_64)
	ui->comboBox->addItem("RAX -> RIP", 1);
	ui->comboBox->addItem("RBX -> RIP", 2);
	ui->comboBox->addItem("RCX -> RIP", 3);
	ui->comboBox->addItem("RDX -> RIP", 4);
	ui->comboBox->addItem("RBP -> RIP", 5);
	ui->comboBox->addItem("RSP -> RIP", 6);
	ui->comboBox->addItem("RSI -> RIP", 7);
	ui->comboBox->addItem("RDI -> RIP", 8);
	ui->comboBox->addItem("R8 -> RIP", 9);
	ui->comboBox->addItem("R9 -> RIP", 10);
	ui->comboBox->addItem("R10 -> RIP", 11);
	ui->comboBox->addItem("R11 -> RIP", 12);
	ui->comboBox->addItem("R12 -> RIP", 13);
	ui->comboBox->addItem("R13 -> RIP", 14);
	ui->comboBox->addItem("R14 -> RIP", 15);
	ui->comboBox->addItem("R15 -> RIP", 16);
	ui->comboBox->addItem("ANY REGISTER -> RIP", 17);
	ui->comboBox->addItem("[RSP] -> RIP", 18);
	ui->comboBox->addItem("[RSP + 8] -> RIP", 19);
	ui->comboBox->addItem("[RSP + 16] -> RIP", 20);
	ui->comboBox->addItem("[RSP - 8] -> RIP", 21);
#endif
}

//------------------------------------------------------------------------------
// Name: ~DialogOpcodes()
// Desc:
//------------------------------------------------------------------------------
DialogOpcodes::~DialogOpcodes() {
	delete ui;
}

//------------------------------------------------------------------------------
// Name: on_listWidget_itemDoubleClicked(QListWidgetItem *item)
// Desc: follows the found item in the data view
//------------------------------------------------------------------------------
void DialogOpcodes::on_listWidget_itemDoubleClicked(QListWidgetItem *item) {
	bool ok;
	const edb::address_t addr = item->data(Qt::UserRole).toULongLong(&ok);
	if(ok) {
		edb::v1::jump_to_address(addr);
	}
}

//------------------------------------------------------------------------------
// Name: showEvent(QShowEvent *)
// Desc:
//------------------------------------------------------------------------------
void DialogOpcodes::showEvent(QShowEvent *) {
	filter_model_->setFilterKeyColumn(3);
	filter_model_->setSourceModel(&edb::v1::memory_regions());
	ui->tableView->setModel(filter_model_);
	ui->progressBar->setValue(0);
	ui->listWidget->clear();
}

//------------------------------------------------------------------------------
// Name: add_result(QList<edb::Instruction> instructions, edb::address_t rva)
// Desc:
//------------------------------------------------------------------------------
void DialogOpcodes::add_result(QList<edb::Instruction> instructions, edb::address_t rva) {
	if(!instructions.isEmpty()) {
		const edb::Instruction insn1 = instructions.takeFirst();

		QString instruction_string = QString("%1: %2").arg(
			edb::v1::format_pointer(rva),
			QString::fromStdString(edisassm::to_string(insn1)));


		Q_FOREACH(const edb::Instruction &instruction, instructions) {
			instruction_string.append(QString("; %1").arg(QString::fromStdString(edisassm::to_string(instruction))));
		}

		QListWidgetItem *const item = new QListWidgetItem(instruction_string);

		item->setData(Qt::UserRole, rva);
		ui->listWidget->addItem(item);
	}
}

//------------------------------------------------------------------------------
// Name: test_reg_to_ip(const DialogOpcodes::OpcodeData &data, edb::address_t start_address)
// Desc:
//------------------------------------------------------------------------------
template <edb::Operand::Register REG>
void DialogOpcodes::test_reg_to_ip(const DialogOpcodes::OpcodeData &data, edb::address_t start_address) {

	const quint8 *p = data.data;
	int len = sizeof(data);

	edb::Instruction insn(p, len, 0, std::nothrow);

	if(insn.valid()) {
		const edb::Operand &op1 = insn.operand(0);
		switch(insn.type()) {
		case edb::Instruction::OP_JMP:
		case edb::Instruction::OP_CALL:
			if(op1.general_type() == edb::Operand::TYPE_REGISTER) {
				if(op1.reg() == REG) {
					add_result((QList<edb::Instruction>() << insn), start_address);
					return;
				}
			}
			break;

		case edb::Instruction::OP_PUSH:
			if(op1.general_type() == edb::Operand::TYPE_REGISTER) {
				if(op1.reg() == REG) {

					p += insn.size();
					len -= insn.size();

					edb::Instruction insn2(p, len, 0, std::nothrow);
					if(insn2.valid()) {
						const edb::Operand &op2 = insn2.operand(0);
						switch(insn2.type()) {
						case edb::Instruction::OP_RET:
							add_result((QList<edb::Instruction>() << insn << insn2), start_address);
							break;
						case edb::Instruction::OP_JMP:
						case edb::Instruction::OP_CALL:

							if(op2.general_type() == edb::Operand::TYPE_EXPRESSION) {

								if(op2.expression().displacement_type == edb::Operand::DISP_NONE) {

									if(op2.expression().base == STACK_REG && op2.expression().index == edb::Operand::REG_NULL) {
										add_result((QList<edb::Instruction>() << insn << insn2), start_address);
										return;
									}

									if(op2.expression().index == STACK_REG && op2.expression().base == edb::Operand::REG_NULL) {
										add_result((QList<edb::Instruction>() << insn << insn2), start_address);
										return;
									}
								}
							}
							break;
						default:
							break;
						}
					}
				}
			}
			break;
		default:
			break;
		}
	}
}

//------------------------------------------------------------------------------
// Name: test_esp_add_0(const OpcodeData &data, edb::address_t start_address)
// Desc:
//------------------------------------------------------------------------------
void DialogOpcodes::test_esp_add_0(const OpcodeData &data, edb::address_t start_address) {

	const quint8 *p = data.data;
	int len = sizeof(data);

	edb::Instruction insn(p, len, 0, std::nothrow);

	if(insn.valid()) {
		const edb::Operand &op1 = insn.operand(0);
		switch(insn.type()) {
		case edb::Instruction::OP_RET:
			add_result((QList<edb::Instruction>() << insn), start_address);
			break;

		case edb::Instruction::OP_CALL:
		case edb::Instruction::OP_JMP:
			if(op1.general_type() == edb::Operand::TYPE_EXPRESSION) {

				if(op1.expression().displacement_type == edb::Operand::DISP_NONE) {

					if(op1.expression().base == STACK_REG && op1.expression().index == edb::Operand::REG_NULL) {
						add_result((QList<edb::Instruction>() << insn), start_address);
						return;
					}

					if(op1.expression().index == STACK_REG && op1.expression().base == edb::Operand::REG_NULL) {
						add_result((QList<edb::Instruction>() << insn), start_address);
						return;
					}
				}
			}
			break;
		case edb::Instruction::OP_POP:
			if(op1.general_type() == edb::Operand::TYPE_REGISTER) {

				p += insn.size();
				len -= insn.size();

				edb::Instruction insn2(p, len, 0, std::nothrow);
				if(insn2.valid()) {
					const edb::Operand &op2 = insn2.operand(0);
					switch(insn2.type()) {
					case edb::Instruction::OP_JMP:
					case edb::Instruction::OP_CALL:

						if(op2.general_type() == edb::Operand::TYPE_REGISTER) {

							if(op1.reg() == op2.reg()) {
								add_result((QList<edb::Instruction>() << insn << insn2), start_address);
							}
						}
						break;
					default:
						break;
					}
				}
			}
			break;

		default:
			break;
		}
	}
}

//------------------------------------------------------------------------------
// Name: test_esp_add_regx1(const OpcodeData &data, edb::address_t start_address)
// Desc:
//------------------------------------------------------------------------------
void DialogOpcodes::test_esp_add_regx1(const OpcodeData &data, edb::address_t start_address) {

	const quint8 *p = data.data;
	int len = sizeof(data);

	edb::Instruction insn(p, len, 0, std::nothrow);

	if(insn.valid()) {
		const edb::Operand &op1 = insn.operand(0);
		switch(insn.type()) {
		case edb::Instruction::OP_POP:

			if(op1.general_type() != edb::Operand::TYPE_REGISTER || op1.reg() != STACK_REG) {
				p += insn.size();
				len -= insn.size();

				edb::Instruction insn2(p, len, 0, std::nothrow);
				if(insn2.valid()) {
					if(insn2.type() == edb::Instruction::OP_RET) {
						add_result((QList<edb::Instruction>() << insn << insn2), start_address);
					}
				}
			}
			break;
		case edb::Instruction::OP_JMP:
		case edb::Instruction::OP_CALL:

			if(op1.general_type() == edb::Operand::TYPE_EXPRESSION) {

				if(op1.displacement() == 4) {
					if(op1.expression().base == STACK_REG && op1.expression().index == edb::Operand::REG_NULL) {
						add_result((QList<edb::Instruction>() << insn), start_address);
					} else if(op1.expression().base == edb::Operand::REG_NULL && op1.expression().index == STACK_REG && op1.expression().scale == 1) {
						add_result((QList<edb::Instruction>() << insn), start_address);
					}

				}
			}
			break;
		case edb::Instruction::OP_SUB:
			if(op1.general_type() == edb::Operand::TYPE_REGISTER && op1.reg() == STACK_REG) {

				const edb::Operand &op2 = insn.operand(1);
				if(op2.general_type() == edb::Operand::TYPE_IMMEDIATE) {

					if(op2.immediate() == -static_cast<int>(sizeof(edb::reg_t))) {
						p += insn.size();
						len -= insn.size();

						edb::Instruction insn2(p, len, 0, std::nothrow);
						if(insn2.valid()) {
							if(insn2.type() == edb::Instruction::OP_RET) {
								add_result((QList<edb::Instruction>() << insn << insn2), start_address);
							}
						}
					}
				}
			}
			break;

		case edb::Instruction::OP_ADD:
			if(op1.general_type() == edb::Operand::TYPE_REGISTER && op1.reg() == STACK_REG) {

				const edb::Operand &op2 = insn.operand(1);
				if(op2.general_type() == edb::Operand::TYPE_IMMEDIATE) {

					if(op2.immediate() == sizeof(edb::reg_t)) {
						p += insn.size();
						len -= insn.size();

						edb::Instruction insn2(p, len, 0, std::nothrow);
						if(insn2.valid()) {
							if(insn2.type() == edb::Instruction::OP_RET) {
								add_result((QList<edb::Instruction>() << insn << insn2), start_address);
							}
						}
					}
				}
			}
			break;

		default:
			break;
		}
	}
}

//------------------------------------------------------------------------------
// Name: test_esp_add_regx2(const OpcodeData &data, edb::address_t start_address)
// Desc:
//------------------------------------------------------------------------------
void DialogOpcodes::test_esp_add_regx2(const OpcodeData &data, edb::address_t start_address) {


	const quint8 *p = data.data;
	int len = sizeof(data);

	edb::Instruction insn(p, len, 0, std::nothrow);

	if(insn.valid()) {
		const edb::Operand &op1 = insn.operand(0);
		switch(insn.type()) {
		case edb::Instruction::OP_POP:

			if(op1.general_type() != edb::Operand::TYPE_REGISTER || op1.reg() != STACK_REG) {
				p += insn.size();
				len -= insn.size();

				edb::Instruction insn2(p, len, 0, std::nothrow);
				if(insn2.valid()) {
					const edb::Operand &op2 = insn2.operand(0);
					switch(insn2.type()) {
					case edb::Instruction::OP_POP:

						if(op2.general_type() != edb::Operand::TYPE_REGISTER || op2.reg() != STACK_REG) {
							p += insn2.size();
							len -= insn2.size();

							edb::Instruction insn3(p, len, 0, std::nothrow);
							if(insn3.valid()) {
								if(insn3.type() == edb::Instruction::OP_RET) {
									add_result((QList<edb::Instruction>() << insn << insn2 << insn3), start_address);
								}
							}
						}
						break;
					default:
						break;
					}
				}
			}
			break;

		case edb::Instruction::OP_JMP:
		case edb::Instruction::OP_CALL:
			if(op1.general_type() == edb::Operand::TYPE_EXPRESSION) {

				if(op1.displacement() == (sizeof(edb::reg_t) * 2)) {
					if(op1.expression().base == STACK_REG && op1.expression().index == edb::Operand::REG_NULL) {
						add_result((QList<edb::Instruction>() << insn), start_address);
					} else if(op1.expression().base == edb::Operand::REG_NULL && op1.expression().index == STACK_REG && op1.expression().scale == 1) {
						add_result((QList<edb::Instruction>() << insn), start_address);
					}

				}
			}
			break;

		case edb::Instruction::OP_SUB:
			if(op1.general_type() == edb::Operand::TYPE_REGISTER && op1.reg() == STACK_REG) {

				const edb::Operand &op2 = insn.operand(1);
				if(op2.general_type() == edb::Operand::TYPE_IMMEDIATE) {

					if(op2.immediate() == -static_cast<int>(sizeof(edb::reg_t) * 2)) {
						p += insn.size();
						len -= insn.size();

						edb::Instruction insn2(p, len, 0, std::nothrow);
						if(insn2.valid()) {
							if(insn2.type() == edb::Instruction::OP_RET) {
								add_result((QList<edb::Instruction>() << insn << insn2), start_address);
							}
						}
					}
				}
			}
			break;

		case edb::Instruction::OP_ADD:
			if(op1.general_type() == edb::Operand::TYPE_REGISTER && op1.reg() == STACK_REG) {

				const edb::Operand &op2 = insn.operand(1);
				if(op2.general_type() == edb::Operand::TYPE_IMMEDIATE) {

					if(op2.immediate() == (sizeof(edb::reg_t) * 2)) {
						p += insn.size();
						len -= insn.size();

						edb::Instruction insn2(p, len, 0, std::nothrow);
						if(insn2.valid()) {
							if(insn2.type() == edb::Instruction::OP_RET) {
								add_result((QList<edb::Instruction>() << insn << insn2), start_address);
							}
						}
					}
				}
			}
			break;

		default:
			break;
		}
	}
}

//------------------------------------------------------------------------------
// Name: test_esp_sub_regx1(const OpcodeData &data, edb::address_t start_address)
// Desc:
//------------------------------------------------------------------------------
void DialogOpcodes::test_esp_sub_regx1(const OpcodeData &data, edb::address_t start_address) {

	const quint8 *p = data.data;
	int len = sizeof(data);

	edb::Instruction insn(p, len, 0, std::nothrow);

	if(insn.valid()) {
		const edb::Operand &op1 = insn.operand(0);
		switch(insn.type()) {
		case edb::Instruction::OP_JMP:
		case edb::Instruction::OP_CALL:
			if(op1.general_type() == edb::Operand::TYPE_EXPRESSION) {

				if(op1.displacement() == -static_cast<int>(sizeof(edb::reg_t))) {
					if(op1.expression().base == STACK_REG && op1.expression().index == edb::Operand::REG_NULL) {
						add_result((QList<edb::Instruction>() << insn), start_address);
					} else if(op1.expression().base == edb::Operand::REG_NULL && op1.expression().index == STACK_REG && op1.expression().scale == 1) {
						add_result((QList<edb::Instruction>() << insn), start_address);
					}

				}
			}
			break;

		case edb::Instruction::OP_SUB:
			if(op1.general_type() == edb::Operand::TYPE_REGISTER && op1.reg() == STACK_REG) {

				const edb::Operand &op2 = insn.operand(1);
				if(op2.general_type() == edb::Operand::TYPE_IMMEDIATE) {

					if(op2.immediate() == static_cast<int>(sizeof(edb::reg_t))) {
						p += insn.size();
						len -= insn.size();

						edb::Instruction insn2(p, len, 0, std::nothrow);
						if(insn2.valid()) {
							if(insn2.type() == edb::Instruction::OP_RET) {
								add_result((QList<edb::Instruction>() << insn << insn2), start_address);
							}
						}
					}
				}
			}
			break;

		case edb::Instruction::OP_ADD:
			if(op1.general_type() == edb::Operand::TYPE_REGISTER && op1.reg() == STACK_REG) {

				const edb::Operand &op2 = insn.operand(1);
				if(op2.general_type() == edb::Operand::TYPE_IMMEDIATE) {

					if(op2.immediate() == -static_cast<int>(sizeof(edb::reg_t))) {
						p += insn.size();
						len -= insn.size();

						edb::Instruction insn2(p, len, 0, std::nothrow);
						if(insn2.valid()) {
							if(insn2.type() == edb::Instruction::OP_RET) {
								add_result((QList<edb::Instruction>() << insn << insn2), start_address);
							}
						}
					}
				}
			}
			break;

		default:
			break;
		}
	}
}

//------------------------------------------------------------------------------
// Name: do_find()
// Desc:
//------------------------------------------------------------------------------
void DialogOpcodes::do_find() {

	const int classtype = ui->comboBox->itemData(ui->comboBox->currentIndex()).toInt();

	const QItemSelectionModel *const selModel = ui->tableView->selectionModel();
	const QModelIndexList sel = selModel->selectedRows();

	if(sel.size() == 0) {
		QMessageBox::information(
			this,
			tr("No Region Selected"),
			tr("You must select a region which is to be scanned for the desired opcode."));
	} else {

		Q_FOREACH(const QModelIndex &selected_item, sel) {

			const QModelIndex index = filter_model_->mapToSource(selected_item);
			const MemRegion *const region = reinterpret_cast<const MemRegion *>(index.internalPointer());

			edb::address_t start_address     = region->start;
			const edb::address_t end_address = region->end;
			const edb::address_t orig_start  = start_address;
			ByteShiftArray bsa(sizeof(OpcodeData));

			// create a reference to the bsa's data so we can pass it to the testXXXX functions
			const OpcodeData &opcode = *reinterpret_cast<const OpcodeData *>(bsa.data());

			// do the search for this region!
			// we intentionally look slightly past the end of the region
			// but we are at that point, we just shift in 0x00's.
			// this will let us find edge case opcodes
			while(start_address < end_address + sizeof(OpcodeData)) {

				// read in the next byte
				quint8 byte;
				if(start_address >= end_address) {
					byte = 0x00;
				} else {
					edb::v1::debugger_core->read_bytes(start_address, &byte, 1);
				}
				bsa <<  byte;

				switch(classtype) {
			#if defined(EDB_X86)
				case 1: test_reg_to_ip<edb::Operand::REG_EAX>(opcode, start_address - (sizeof(OpcodeData) - 1)); break;
				case 2: test_reg_to_ip<edb::Operand::REG_EBX>(opcode, start_address - (sizeof(OpcodeData) - 1)); break;
				case 3: test_reg_to_ip<edb::Operand::REG_ECX>(opcode, start_address - (sizeof(OpcodeData) - 1)); break;
				case 4: test_reg_to_ip<edb::Operand::REG_EDX>(opcode, start_address - (sizeof(OpcodeData) - 1)); break;
				case 5: test_reg_to_ip<edb::Operand::REG_EBP>(opcode, start_address - (sizeof(OpcodeData) - 1)); break;
				case 6: test_reg_to_ip<edb::Operand::REG_ESP>(opcode, start_address - (sizeof(OpcodeData) - 1)); break;
				case 7: test_reg_to_ip<edb::Operand::REG_ESI>(opcode, start_address - (sizeof(OpcodeData) - 1)); break;
				case 8: test_reg_to_ip<edb::Operand::REG_EDI>(opcode, start_address - (sizeof(OpcodeData) - 1)); break;
			#elif defined(EDB_X86_64)
				case 1: test_reg_to_ip<edb::Operand::REG_RAX>(opcode, start_address - (sizeof(OpcodeData) - 1)); break;
				case 2: test_reg_to_ip<edb::Operand::REG_RBX>(opcode, start_address - (sizeof(OpcodeData) - 1)); break;
				case 3: test_reg_to_ip<edb::Operand::REG_RCX>(opcode, start_address - (sizeof(OpcodeData) - 1)); break;
				case 4: test_reg_to_ip<edb::Operand::REG_RDX>(opcode, start_address - (sizeof(OpcodeData) - 1)); break;
				case 5: test_reg_to_ip<edb::Operand::REG_RBP>(opcode, start_address - (sizeof(OpcodeData) - 1)); break;
				case 6: test_reg_to_ip<edb::Operand::REG_RSP>(opcode, start_address - (sizeof(OpcodeData) - 1)); break;
				case 7: test_reg_to_ip<edb::Operand::REG_RSI>(opcode, start_address - (sizeof(OpcodeData) - 1)); break;
				case 8: test_reg_to_ip<edb::Operand::REG_RDI>(opcode, start_address - (sizeof(OpcodeData) - 1)); break;
				case 9: test_reg_to_ip<edb::Operand::REG_R8>(opcode, start_address - (sizeof(OpcodeData) - 1)); break;
				case 10: test_reg_to_ip<edb::Operand::REG_R9>(opcode, start_address - (sizeof(OpcodeData) - 1)); break;
				case 11: test_reg_to_ip<edb::Operand::REG_R10>(opcode, start_address - (sizeof(OpcodeData) - 1)); break;
				case 12: test_reg_to_ip<edb::Operand::REG_R11>(opcode, start_address - (sizeof(OpcodeData) - 1)); break;
				case 13: test_reg_to_ip<edb::Operand::REG_R12>(opcode, start_address - (sizeof(OpcodeData) - 1)); break;
				case 14: test_reg_to_ip<edb::Operand::REG_R13>(opcode, start_address - (sizeof(OpcodeData) - 1)); break;
				case 15: test_reg_to_ip<edb::Operand::REG_R14>(opcode, start_address - (sizeof(OpcodeData) - 1)); break;
				case 16: test_reg_to_ip<edb::Operand::REG_R15>(opcode, start_address - (sizeof(OpcodeData) - 1)); break;
			#endif

				case 17:
				#if defined(EDB_X86)
					test_reg_to_ip<edb::Operand::REG_EAX>(opcode, start_address - (sizeof(OpcodeData) - 1));
					test_reg_to_ip<edb::Operand::REG_EBX>(opcode, start_address - (sizeof(OpcodeData) - 1));
					test_reg_to_ip<edb::Operand::REG_ECX>(opcode, start_address - (sizeof(OpcodeData) - 1));
					test_reg_to_ip<edb::Operand::REG_EDX>(opcode, start_address - (sizeof(OpcodeData) - 1));
					test_reg_to_ip<edb::Operand::REG_EBP>(opcode, start_address - (sizeof(OpcodeData) - 1));
					test_reg_to_ip<edb::Operand::REG_ESP>(opcode, start_address - (sizeof(OpcodeData) - 1));
					test_reg_to_ip<edb::Operand::REG_ESI>(opcode, start_address - (sizeof(OpcodeData) - 1));
					test_reg_to_ip<edb::Operand::REG_EDI>(opcode, start_address - (sizeof(OpcodeData) - 1));
				#elif defined(EDB_X86_64)
					test_reg_to_ip<edb::Operand::REG_RAX>(opcode, start_address - (sizeof(OpcodeData) - 1));
					test_reg_to_ip<edb::Operand::REG_RBX>(opcode, start_address - (sizeof(OpcodeData) - 1));
					test_reg_to_ip<edb::Operand::REG_RCX>(opcode, start_address - (sizeof(OpcodeData) - 1));
					test_reg_to_ip<edb::Operand::REG_RDX>(opcode, start_address - (sizeof(OpcodeData) - 1));
					test_reg_to_ip<edb::Operand::REG_RBP>(opcode, start_address - (sizeof(OpcodeData) - 1));
					test_reg_to_ip<edb::Operand::REG_RSP>(opcode, start_address - (sizeof(OpcodeData) - 1));
					test_reg_to_ip<edb::Operand::REG_RSI>(opcode, start_address - (sizeof(OpcodeData) - 1));
					test_reg_to_ip<edb::Operand::REG_RDI>(opcode, start_address - (sizeof(OpcodeData) - 1));
					test_reg_to_ip<edb::Operand::REG_R8>(opcode, start_address - (sizeof(OpcodeData) - 1));
					test_reg_to_ip<edb::Operand::REG_R9>(opcode, start_address - (sizeof(OpcodeData) - 1));
					test_reg_to_ip<edb::Operand::REG_R10>(opcode, start_address - (sizeof(OpcodeData) - 1));
					test_reg_to_ip<edb::Operand::REG_R11>(opcode, start_address - (sizeof(OpcodeData) - 1));
					test_reg_to_ip<edb::Operand::REG_R12>(opcode, start_address - (sizeof(OpcodeData) - 1));
					test_reg_to_ip<edb::Operand::REG_R13>(opcode, start_address - (sizeof(OpcodeData) - 1));
					test_reg_to_ip<edb::Operand::REG_R14>(opcode, start_address - (sizeof(OpcodeData) - 1));
					test_reg_to_ip<edb::Operand::REG_R15>(opcode, start_address - (sizeof(OpcodeData) - 1));
				#endif
					break;
				case 18:
					// [ESP] -> EIP
					test_esp_add_0(opcode, start_address - (sizeof(OpcodeData) - 1));
					break;
				case 19:
					// [ESP + 4] -> EIP
					test_esp_add_regx1(opcode, start_address - (sizeof(OpcodeData) - 1));
					break;
				case 20:
					// [ESP + 8] -> EIP
					test_esp_add_regx2(opcode, start_address - (sizeof(OpcodeData) - 1));
					break;
				case 21:
					// [ESP - 4] -> EIP
					test_esp_sub_regx1(opcode, start_address - (sizeof(OpcodeData) - 1));
					break;
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
void DialogOpcodes::on_btnFind_clicked() {
	ui->btnFind->setEnabled(false);
	ui->listWidget->clear();
	ui->progressBar->setValue(0);
	do_find();
	ui->progressBar->setValue(100);
	ui->btnFind->setEnabled(true);
}
