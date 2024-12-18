#include "product.h"

// Default Constructor
Product::Product()
    : id(0), name(""), price(0.0), stock(0), description(""), discount(0.0) {}

// Parameterized Constructor
Product::Product(int id, QString name, double price, int stock, QString description, double discount)
    : id(id), name(name), price(price), stock(stock), description(description), discount(discount) {}

// Getters
int Product::getId() const {
    return id;
}

QString Product::getName() const {
    return name;
}

double Product::getPrice() const {
    return price;
}

int Product::getStock() const {
    return stock;
}

QString Product::getDescription() const {
    return description;
}

double Product::getDiscount() const {
    return discount;
}

// Setters
void Product::setName(const QString &newName) {
    name = newName;
}

void Product::setPrice(double newPrice) {
    if (newPrice >= 0) {
        price = newPrice;
    }
}

void Product::setStock(int newStock) {
    if (newStock >= 0) {
        stock = newStock;
    }
}

void Product::setDescription(const QString &newDescription) {
    description = newDescription;
}

void Product::setDiscount(double discountPercentage) {
    if (discountPercentage >= 0 && discountPercentage <= 100) {
        discount = discountPercentage;
    }
}

// Calculate price after applying discount
double Product::calculateFinalPrice(int quantity) const {
    // Allow quantity to influence the final price (if needed for bulk pricing or promo discounts)
    return price * quantity * (1 - discount / 100.0);
}

// Return product details as a string
QString Product::getDetails() const {
    return QString("ID: %1\nName: %2\nPrice: %3\nStock: %4\nDescription: %5\nDiscount: %6%%\nFinal Price (1 unit): %7")
            .arg(id)
            .arg(name)
            .arg(price, 0, 'f', 2)
            .arg(stock)
            .arg(description)
            .arg(discount, 0, 'f', 2)
            .arg(calculateFinalPrice(1), 0, 'f', 2);
}

// New method to return a concise representation of the product
QString Product::toString() const {
    return QString("%1 - $%2").arg(name).arg(price, 0, 'f', 2);
}
