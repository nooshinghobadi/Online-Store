#ifndef ORDER_H
#define ORDER_H

#include <QString>
#include <QList>
#include "product.h"

class Order {
private:
    int id;                      // Unique identifier for the order
    QString date;                // Date of the order
    QList<Product> products;     // List of products in the order
    QList<int> quantities;       // List of product quantities corresponding to each product
    double totalPrice;           // Total price of the order
    QString status;              // Status of the order (e.g., "Pending", "Completed")

public:
    // Constructor
    Order(int id, const QString &date, const QList<Product> &products, const QList<int> &quantities, double totalPrice, const QString &status = "Pending");

    // Getters
    int getId() const;
    QString getDate() const;
    QList<Product> getProducts() const;
    QList<int> getQuantities() const;
    double getTotalPrice() const;
    QString getStatus() const;

    // Setters
    void setDate(const QString &newDate);
    void setStatus(const QString &newStatus);

    // Other member functions
    QString getOrderDetails() const;  // Return order details as a formatted string
    void updateStatus(const QString &newStatus);  // Update order status (e.g., "Shipped")
    double calculateTotalPrice() const;  // Calculate the total price considering quantities
};

#endif // ORDER_H
