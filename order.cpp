#include "order.h"

// Constructor
Order::Order(int id, const QString &date, const QList<Product> &products, const QList<int> &quantities, double totalPrice, const QString &status)
    : id(id), date(date), products(products), quantities(quantities), totalPrice(totalPrice), status(status) {}

// Getters
int Order::getId() const {
    return id;
}

QString Order::getDate() const {
    return date;
}

QList<Product> Order::getProducts() const {
    return products;
}

QList<int> Order::getQuantities() const {
    return quantities;
}

double Order::getTotalPrice() const {
    return totalPrice;
}

QString Order::getStatus() const {
    return status;
}

// Setters
void Order::setDate(const QString &newDate) {
    date = newDate;
}

void Order::setStatus(const QString &newStatus) {
    status = newStatus;
}

// Return order details as a formatted string
QString Order::getOrderDetails() const {
    QString details = QString("Order ID: %1\nDate: %2\nStatus: %3\nTotal Price: %4\n")
                      .arg(id)
                      .arg(date)
                      .arg(status)
                      .arg(totalPrice);

    details += "Products:\n";
    for (int i = 0; i < products.size(); ++i) {
        details += QString("Product: %1 x %2\n").arg(products[i].getName()).arg(quantities[i]);
    }

    return details;
}

// Update the order status (e.g., for shipping or completion)
void Order::updateStatus(const QString &newStatus) {
    status = newStatus;
}

// Calculate the final total price (including discounts)
double Order::calculateTotalPrice() const {
    double finalPrice = 0;
    for (int i = 0; i < products.size(); ++i) {
        finalPrice += products[i].calculateFinalPrice() * quantities[i];
    }
    return finalPrice;
}
