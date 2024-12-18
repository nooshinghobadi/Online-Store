#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "product.h"
#include "cart.h"
#include "order.h"
#include "database.h"
#include <QDate>  // Include QDate to handle the current date
#include <QMessageBox>  // Include for displaying messages
#include <QInputDialog>  // Include for input dialogs

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;  // UI pointer to the generated UI
    Cart cart;           // Shopping cart object

    // Helper methods
    void loadProducts();            // Load all products from the database
    void displayCart();             // Display cart details in a widget (if needed)
    void clearCart();               // Clear the cart after finalizing order
    void finalizeOrder();           // Finalize the current order and save it to the database
    void loadOrders();              // Load all orders for viewing the transaction history

private slots:
    // Cart-related actions
    void onAddToCartClicked();      // Handle adding a product to the cart
    void onViewCartClicked();       // Handle viewing the cart
    void onFinalizeOrderClicked();  // Handle order finalization
    void onApplyDiscountClicked();  // Handle discount application to the cart

    // Inventory management actions
    Product getSelectedProduct();
    void onAddProductClicked();     // Handle adding a new product to the inventory
    void onEditProductClicked();    // Handle editing an existing product in the inventory
    void onRemoveProductClicked();  // Handle removing a product from the inventory

    // Transaction history actions
    void onViewOrdersClicked();     // Handle viewing past orders

    void onRemoveProductFromCartClicked();
    void onApplyPromoCodeClicked();


};

#endif // MAINWINDOW_H
