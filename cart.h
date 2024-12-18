#ifndef CART_H
#define CART_H

#include <QMap>
#include <QList>
#include "product.h"
#include "database.h"

class Cart {
private:
    QMap<int, int> productQuantities;  // Store product IDs and their quantities
    double totalPrice;                // Total price of all items in the cart
    double discount;                  // Discount percentage applied to the cart

    void calculateTotalPrice();       // Helper function to calculate the total price

public:
    // Constructor
    Cart();

    // Add a product to the cart with specified quantity
    bool addProduct(Product &product, int quantity);

    // Remove a product entirely or decrease its quantity
    bool removeProduct(int productId, int quantity = 0);

    // Clear the cart
    void clearCart();

    // Set a discount percentage for the entire cart
    void setDiscount(double discountPercentage);

    // Apply a promo code
    bool applyPromoCode(const QString &promoCode);

    // Calculate total price of all items in the cart
    double getTotalPrice() const;

    // Get cart details as a formatted string (for GUI display)
    QString displayCartDetails() const;

    // Get all products and their quantities in the cart
    QList<Product> getProducts() const;  // Fetch products using product IDs

    // Get all products and their quantities in the cart as a map (for cart management)
    QMap<int, int> getProductQuantities() const;
};

#endif // CART_H
