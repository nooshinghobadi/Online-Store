#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDate>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDebug>
#include <QInputDialog>
#include <QMainWindow>
#include <QStackedWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QTableWidget>
#include <QLineEdit>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);  // Set up the UI components

    // Connect buttons to their respective slots
    connect(ui->addToCartButton, &QPushButton::clicked, this, &MainWindow::onAddToCartClicked);
    connect(ui->viewCartButton, &QPushButton::clicked, this, &MainWindow::onViewCartClicked);
    connect(ui->finalizeOrderButton, &QPushButton::clicked, this, &MainWindow::onFinalizeOrderClicked);
    connect(ui->applyDiscountButton, &QPushButton::clicked, this, &MainWindow::onApplyDiscountClicked);
    connect(ui->applyPromoCodeButton, &QPushButton::clicked, this, &MainWindow::onApplyPromoCodeClicked);
    connect(ui->addProductButton, &QPushButton::clicked, this, &MainWindow::onAddProductClicked);
    connect(ui->editProductButton, &QPushButton::clicked, this, &MainWindow::onEditProductClicked);
    connect(ui->removeProductButton, &QPushButton::clicked, this, &MainWindow::onRemoveProductClicked);
    connect(ui->viewOrdersButton, &QPushButton::clicked, this, &MainWindow::onViewOrdersClicked);
    connect(ui->removeProductFromCartButton, &QPushButton::clicked, this, &MainWindow::onRemoveProductFromCartClicked);



    loadProducts();  // Load products from the database
}

MainWindow::~MainWindow() {
    delete ui;  // Clean up the ui object
}

void MainWindow::loadProducts() {
    QList<Product> products = Database::getAllProducts(); // Fetch all products from the database
    ui->productTable->setRowCount(0); // Clear any existing rows in the table

    // Add each product to the table
    for (int i = 0; i < products.size(); ++i) {
        const Product &product = products[i];
        ui->productTable->insertRow(i);
        ui->productTable->setItem(i, 0, new QTableWidgetItem(QString::number(product.getId())));
        ui->productTable->setItem(i, 1, new QTableWidgetItem(product.getName()));
        ui->productTable->setItem(i, 2, new QTableWidgetItem(QString::number(product.getPrice(), 'f', 2)));
        ui->productTable->setItem(i, 3, new QTableWidgetItem(QString::number(product.getStock())));
        ui->productTable->setItem(i, 4, new QTableWidgetItem(product.getDescription()));
    }

    // Adjust the table's appearance
    ui->productTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

Product MainWindow::getSelectedProduct() {
    int selectedRow = ui->productTable->currentRow();
    if (selectedRow < 0) {
        return Product();
    }

    int id = ui->productTable->item(selectedRow, 0)->text().toInt();
    QString name = ui->productTable->item(selectedRow, 1)->text();
    double price = ui->productTable->item(selectedRow, 2)->text().toDouble();
    int stock = ui->productTable->item(selectedRow, 3)->text().toInt();
    QString description = ui->productTable->item(selectedRow, 4)->text();

    Product product(id, name, price, stock, description);

    return product;
}

// Slot for adding a product to the cart
void MainWindow::onAddToCartClicked() {
    Product product = getSelectedProduct();
    if (product.getId() == 0) {
        QMessageBox::warning(this, "No Product Selected", "Please select a product to add to the cart.");
        return;
    }

    // Ask for the quantity of the product to add to the cart
    bool ok;
    int quantity = QInputDialog::getInt(this, tr("Enter Quantity"), tr("Quantity:"), 1, 1, product.getStock(), 1, &ok);

    if (!ok || quantity <= 0) {
        return;  // If quantity is invalid or user canceled, do nothing
    }

    bool added = cart.addProduct(product, quantity);

    if (added) {
        QMessageBox::information(this, "Product Added", "Product has been added to the cart.");
        loadProducts();
    } else {
        QMessageBox::warning(this, "Add Product", "Not enough stock available.");
    }
}

// Slot for viewing the cart
void MainWindow::onViewCartClicked() {
    if (cart.getProductQuantities().isEmpty()) {
        QMessageBox::information(this, "Cart Empty", "Your cart is empty.");
    } else {
        QString cartDetails = cart.displayCartDetails();
        QMessageBox::information(this, "Cart Details", cartDetails);
    }
}

// Slot for finalizing the order
void MainWindow::onFinalizeOrderClicked() {
    finalizeOrder();  // Call the method to finalize the order
}

// Finalize the order and save it to the database
void MainWindow::finalizeOrder() {
    if (cart.getProductQuantities().isEmpty()) {
        QMessageBox::warning(this, "Cart Empty", "Your cart is empty. Add products to the cart before finalizing the order.");
        return;
    }

    // Get the current date for the order
    QString date = QDate::currentDate().toString("yyyy-MM-dd");

    // Calculate the total price of the cart
    double totalPrice = cart.getTotalPrice();

    // Get the list of products and their quantities from the cart
    QList<Product> products = cart.getProducts();
    QList<int> quantities;  // Create an empty list to store quantities
    for (const Product &product : products) {
        quantities.append(cart.getProductQuantities()[product.getId()]);
    }

    // Create an order from the cart
    Order order(0, date, products, quantities, totalPrice, "Pending");

    // Save the order to the database
    if (Database::addOrder(order)) {
        for (Product &product : products) {
            product.setStock(product.getStock() - cart.getProductQuantities()[product.getId()]);
            Database::updateProduct(product);
        }
        loadProducts();

        QMessageBox::information(this, "Order Finalized", "Your order has been successfully placed!");
        cart.clearCart();  // Clear the cart after finalizing the order
    } else {
        QMessageBox::critical(this, "Order Error", "Failed to place the order.");
    }
}

// Slot for applying a discount
void MainWindow::onApplyDiscountClicked() {
    bool ok;
    double discount = QInputDialog::getDouble(this, tr("Apply Discount"), tr("Discount Percentage:"), 0, 0, 100, 1, &ok);

    if (ok && discount >= 0 && discount <= 100) {
        cart.setDiscount(discount);  // Apply the discount to the cart
        QMessageBox::information(this, "Discount Applied", QString("A discount of %1% has been applied.").arg(discount));
    } else {
        QMessageBox::warning(this, "Invalid Discount", "Please enter a valid discount percentage (0-100).");
    }
}

// Slot for adding a new product (Inventory Management)
void MainWindow::onAddProductClicked() {
    bool ok;
    QString name = QInputDialog::getText(this, tr("Add Product"), tr("Product Name:"), QLineEdit::Normal, "", &ok);
    if (!ok || name.isEmpty()) return;

    double price = QInputDialog::getDouble(this, tr("Add Product"), tr("Product Price:"), 0, 0, 10000, 2, &ok);
    if (!ok || price <= 0) return;

    int stock = QInputDialog::getInt(this, tr("Add Product"), tr("Product Stock:"), 0, 0, 1000, 1, &ok);
    if (!ok || stock <= 0) return;

    QString description = QInputDialog::getText(this, tr("Add Product"), tr("Product Description:"), QLineEdit::Normal, "", &ok);
    if (!ok || description.isEmpty()) return;

    Product newProduct(0, name, price, stock, description);
    if (Database::addProduct(newProduct)) {
        QMessageBox::information(this, "Product Added", "New product added successfully!");
        loadProducts();  // Refresh the product table
    } else {
        QMessageBox::critical(this, "Error", "Failed to add the product.");
    }
}

// Slot for editing a product (Inventory Management)
void MainWindow::onEditProductClicked() {
    int selectedRow = ui->productTable->currentRow();
    if (selectedRow < 0) {
        QMessageBox::warning(this, "No Product Selected", "Please select a product to edit.");
        return;
    }

    QString name = ui->productTable->item(selectedRow, 1)->text();
    double price = ui->productTable->item(selectedRow, 2)->text().toDouble();
    int stock = ui->productTable->item(selectedRow, 3)->text().toInt();
    QString description = ui->productTable->item(selectedRow, 4)->text();

    // Allow the user to edit the product
    bool ok;
    QString newName = QInputDialog::getText(this, tr("Edit Product"), tr("Product Name:"), QLineEdit::Normal, name, &ok);
    if (!ok || newName.isEmpty()) return;

    double newPrice = QInputDialog::getDouble(this, tr("Edit Product"), tr("Product Price:"), price, 0, 10000, 2, &ok);
    if (!ok || newPrice <= 0) return;

    int newStock = QInputDialog::getInt(this, tr("Edit Product"), tr("Product Stock:"), stock, 0, 1000, 1, &ok);
    if (!ok || newStock <= 0) return;

    QString newDescription = QInputDialog::getText(this, tr("Edit Product"), tr("Product Description:"), QLineEdit::Normal, description, &ok);
    if (!ok || newDescription.isEmpty()) return;

    Product updatedProduct(ui->productTable->item(selectedRow, 0)->text().toInt(), newName, newPrice, newStock, newDescription);

    if (Database::updateProduct(updatedProduct)) {
        QMessageBox::information(this, "Product Updated", "Product details updated successfully!");
        loadProducts();  // Refresh the product table
    } else {
        QMessageBox::critical(this, "Error", "Failed to update the product.");
    }
}

// Slot for removing a product (Inventory Management)
void MainWindow::onRemoveProductClicked() {
    int selectedRow = ui->productTable->currentRow();
    if (selectedRow < 0) {
        QMessageBox::warning(this, "No Product Selected", "Please select a product to remove.");
        return;
    }

    int productId = ui->productTable->item(selectedRow, 0)->text().toInt();

    if (Database::deleteProduct(productId)) {
        QMessageBox::information(this, "Product Removed", "Product removed successfully!");
        loadProducts();  // Refresh the product table
    } else {
        QMessageBox::critical(this, "Error", "Failed to remove the product.");
    }
}

// Slot for viewing the transaction history
void MainWindow::onViewOrdersClicked() {
    QList<Order> orders = Database::getAllOrders();
    if (orders.isEmpty()) {
        QMessageBox::information(this, "No Orders", "There are no past orders.");
    } else {
        QString orderDetails;
        foreach (const Order &order, orders) {
            orderDetails += order.getOrderDetails() + "\n\n";
        }
        QMessageBox::information(this, "Order History", orderDetails);
    }
}

// Slot for removing a product from the cart
void MainWindow::onRemoveProductFromCartClicked() {
    Product product = getSelectedProduct();
    if (product.getId() == 0) {
        QMessageBox::warning(this, "No Product Selected", "Please select a product to remove from the cart.");
        return;
    }

    // Ask for the quantity to remove
    bool ok;
    int quantity = QInputDialog::getInt(this, tr("Enter Quantity to Remove"), tr("Quantity:"), 1, 1, cart.getProductQuantities()[product.getId()], 1, &ok);

    if (!ok || quantity <= 0) {
        return;  // Do nothing if invalid input or canceled
    }

    // Call the `removeProduct` method in the Cart class
    if (cart.removeProduct(product.getId(), quantity)) {
        QMessageBox::information(this, "Product Updated", "Product quantity has been updated in the cart.");
    } else {
        QMessageBox::warning(this, "Operation Failed", "Failed to update the cart. Product may not exist.");
    }

    // Refresh the product table and cart details
    loadProducts();
}

// Slot for applying a promo code
void MainWindow::onApplyPromoCodeClicked() {
    // Prompt the user to enter a promo code
    bool ok;
    QString promoCode = QInputDialog::getText(this, tr("Apply Promo Code"), tr("Enter Promo Code:"), QLineEdit::Normal, "", &ok);

    if (!ok || promoCode.isEmpty()) {
        QMessageBox::warning(this, "No Promo Code", "Please enter a promo code.");
        return;
    }

    // Attempt to apply the promo code
    if (cart.applyPromoCode(promoCode)) {
        QMessageBox::information(this, "Promo Code Applied", QString("Promo code '%1' applied successfully!").arg(promoCode));
    } else {
        QMessageBox::warning(this, "Invalid Promo Code", QString("Promo code '%1' is not valid.").arg(promoCode));
    }
}
