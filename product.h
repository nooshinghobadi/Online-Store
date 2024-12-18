#ifndef PRODUCT_H
#define PRODUCT_H

#include <QString>

class Product {
private:
    int id;             // Unique identifier for the product
    QString name;       // Product name
    double price;       // Price of the product
    int stock;          // Stock count
    QString description;// Product description
    double discount;    // Discount percentage

public:
    // Default Constructor
    Product();

    // Parameterized Constructor
    Product(int id, QString name, double price, int stock, QString description, double discount = 0);

    // Getters
    int getId() const;
    QString getName() const;
    double getPrice() const;
    int getStock() const;
    QString getDescription() const;
    double getDiscount() const;

    // Setters
    void setName(const QString &newName);
    void setPrice(double newPrice);
    void setStock(int newStock);
    void setDescription(const QString &newDescription);
    void setDiscount(double discountPercentage);

    // Other member functions
    double calculateFinalPrice(int quantity = 1) const; // Calculate price after applying discount (supports quantity)
    QString getDetails() const;                        // Return product details as a string
    QString toString() const;                          // Simple string representation for display
};

#endif // PRODUCT_H
