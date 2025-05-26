#!/usr/bin/env python3

import sys
import os
import json
import subprocess
from PyQt5.QtWidgets import (QApplication, QMainWindow, QWidget, QVBoxLayout,
                           QHBoxLayout, QPushButton, QLabel, QTreeWidget,
                           QTreeWidgetItem, QProgressBar, QMessageBox,
                           QLineEdit)
from PyQt5.QtCore import Qt, QThread, pyqtSignal

class PackageInstaller(QThread):
    progress = pyqtSignal(int)
    finished = pyqtSignal()
    error = pyqtSignal(str)

    def __init__(self, packages):
        super().__init__()
        self.packages = packages

    def run(self):
        try:
            total = len(self.packages)
            for i, package in enumerate(self.packages):
                # Install package using pacman
                cmd = f"sudo pacman -S --noconfirm {package}"
                process = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
                stdout, stderr = process.communicate()
                
                if process.returncode != 0:
                    self.error.emit(f"Error installing {package}: {stderr.decode()}")
                    return
                
                self.progress.emit(int((i + 1) / total * 100))
            
            self.finished.emit()
        except Exception as e:
            self.error.emit(str(e))

class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("BlackArch GUI Installer")
        self.setMinimumSize(800, 600)
        
        # Create main widget and layout
        main_widget = QWidget()
        self.setCentralWidget(main_widget)
        layout = QVBoxLayout(main_widget)
        
        # Create search box
        search_layout = QHBoxLayout()
        self.search_label = QLabel("Search Package:")
        self.search_input = QLineEdit()
        self.search_input.setPlaceholderText("Type package name...")
        self.search_input.textChanged.connect(self.filter_packages)
        search_layout.addWidget(self.search_label)
        search_layout.addWidget(self.search_input)
        layout.addLayout(search_layout)
        
        # Create package tree
        self.tree = QTreeWidget()
        self.tree.setHeaderLabels(["Package", "Description"])
        self.tree.setColumnWidth(0, 300)
        layout.addWidget(self.tree)
        
        # Create progress bar
        self.progress = QProgressBar()
        layout.addWidget(self.progress)
        
        # Create buttons
        button_layout = QHBoxLayout()
        self.install_button = QPushButton("Install Selected")
        self.install_button.clicked.connect(self.install_packages)
        button_layout.addWidget(self.install_button)
        
        self.refresh_button = QPushButton("Refresh")
        self.refresh_button.clicked.connect(self.load_packages)
        button_layout.addWidget(self.refresh_button)
        
        layout.addLayout(button_layout)
        
        # Store all packages for filtering
        self.all_packages = []
        
        # Load packages
        self.load_packages()

    def filter_packages(self):
        """Filter packages based on search text"""
        search_text = self.search_input.text().lower()
        
        # Clear existing items
        self.tree.clear()
        
        # Create category dictionary
        categories = {}
        
        # Filter and organize packages
        for package in self.all_packages:
            name, desc, category = package
            if search_text in name.lower() or search_text in desc.lower():
                if category not in categories:
                    category_item = QTreeWidgetItem(self.tree)
                    category_item.setText(0, category)
                    category_item.setCheckState(0, Qt.Unchecked)
                    categories[category] = category_item
                
                package_item = QTreeWidgetItem(categories[category])
                package_item.setText(0, name)
                package_item.setText(1, desc)
                package_item.setCheckState(0, Qt.Unchecked)
        
        self.tree.expandAll()

    def load_packages(self):
        """Load available packages from pacman"""
        try:
            # Get package list
            cmd = "pacman -Ss blackarch"
            process = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE)
            output = process.stdout.read().decode()
            
            # Clear existing items and packages list
            self.tree.clear()
            self.all_packages = []
            
            # Parse and add packages
            current_category = None
            for line in output.splitlines():
                if line.startswith("blackarch/"):
                    name, desc = line.split(" ", 1)
                    category = name.split("/")[1].split("-")[0]
                    
                    # Store package info for filtering
                    self.all_packages.append((name, desc, category))
                    
                    if category != current_category:
                        current_category = category
                        category_item = QTreeWidgetItem(self.tree)
                        category_item.setText(0, category)
                        category_item.setCheckState(0, Qt.Unchecked)
                    
                    package_item = QTreeWidgetItem(category_item)
                    package_item.setText(0, name)
                    package_item.setText(1, desc)
                    package_item.setCheckState(0, Qt.Unchecked)
            
            self.tree.expandAll()
            
        except Exception as e:
            QMessageBox.critical(self, "Error", f"Failed to load packages: {str(e)}")

    def install_packages(self):
        """Install selected packages"""
        selected_packages = []
        
        # Get selected packages
        def get_selected_items(item):
            if item.checkState(0) == Qt.Checked:
                selected_packages.append(item.text(0))
            for i in range(item.childCount()):
                get_selected_items(item.child(i))
        
        for i in range(self.tree.topLevelItemCount()):
            get_selected_items(self.tree.topLevelItem(i))
        
        if not selected_packages:
            QMessageBox.warning(self, "Warning", "No packages selected")
            return
        
        # Start installation
        self.installer = PackageInstaller(selected_packages)
        self.installer.progress.connect(self.progress.setValue)
        self.installer.finished.connect(self.installation_finished)
        self.installer.error.connect(self.installation_error)
        
        self.install_button.setEnabled(False)
        self.refresh_button.setEnabled(False)
        self.installer.start()

    def installation_finished(self):
        """Handle installation completion"""
        self.install_button.setEnabled(True)
        self.refresh_button.setEnabled(True)
        QMessageBox.information(self, "Success", "Installation completed successfully")
        self.load_packages()

    def installation_error(self, error):
        """Handle installation errors"""
        self.install_button.setEnabled(True)
        self.refresh_button.setEnabled(True)
        QMessageBox.critical(self, "Error", f"Installation failed: {error}")

if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = MainWindow()
    window.show()
    sys.exit(app.exec_()) 