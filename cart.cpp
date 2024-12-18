#include "cart.h"
#include <QString>
#include <QDebug>

// Constructor
Cart::Cart() : totalPrice(0), discount(0) {}

// Add a product to the cart with specified quantity
bool Cart::addProduct(Product &product, int quantity) {
    if (product.getStock() < quantity) {
        qDebug() << "Not enough stock for product: " << product.getName();
        return false; // Fail if requested quantity exceeds stock
    }

    // If the product already exists in the cart, update the quantity
    if (productQuantities.contains(product.getId())) {
        int newQuantity = productQuantities[product.getId()] + quantity;
        if (newQuantity > product.getStock()) {
            qDebug() << "Not enough stock to add more of product: " << product.getName();
            return false;
        }
        productQuantities[product.getId()] = newQuantity;
    } else {
        productQuantities[product.getId()] = quantity;
    }

    calculateTotalPrice();

    return true;
}

// Remove a product entirely or decrease its quantity
bool Cart::removeProduct(int productId, int quantity) {
    if (!productQuantities.contains(productId)) {
        qDebug() << "Product ID not found in cart: " << productId;
        return false; // Product not found in the cart
    }

    if (quantity == 0 || productQuantities[productId] <= quantity) {
        productQuantities.remove(productId); // Remove the product completely
    } else {
        productQuantities[productId] -= quantity; // Decrease the quantity
    }

    calculateTotalPrice();
    return true;
}

// Clear the cart
void Cart::clearCart() {
    productQuantities.clear();
    totalPrice = 0;
    discount = 0;
}

// Apply a discount percentage to the cart
void Cart::setDiscount(double discountPercentage) {
    if (discountPercentage >= 0 && discountPercentage <= 100) {
        discount = discountPercentage;
        calculateTotalPrice();
    }
}

// Apply a promo code
bool Cart::applyPromoCode(const QString &promoCode) {
    if (promoCode == "SAVE10") {
        setDiscount(10); // 10% discount
    } else if (promoCode == "FALL20") {
        setDiscount(20); // 20% discount
    } else {
        qDebug() << "Invalid promo code: " << promoCode;
        return false;
    }
    return true;
}

// Private helper function to calculate the total price of all items in the cart
void Cart::calculateTotalPrice() {
    totalPrice = 0;

    for (auto it = productQuantities.begin(); it != productQuantities.end(); ++it) {
        int productId = it.key();
        int quantity = it.value();

        // Fetch product details from database using product ID
        Product product = Database::getProductById(productId); // Assuming a method in Database class to fetch product by ID
        double priceAfterDiscount = product.getPrice() * (1 - product.getDiscount() / 100.0);

        totalPrice += priceAfterDiscount * quantity;
    }

    // Apply cart-wide discount
    totalPrice *= (1 - discount / 100.0);
}

// Get total price
double Cart::getTotalPrice() const {
    return totalPrice;
}

// Get cart details as a formatted string (for GUI display)
QString Cart::displayCartDetails() const {
    QString details;
    for (auto it = productQuantities.begin(); it != productQuantities.end(); ++it) {
        int productId = it.key();
        int quantity = it.value();

        // Fetch product details from database using product ID
        Product product = Database::getProductById(productId); // Assuming a method in Database class to fetch product by ID
        details += QString("%1 x %2: %3\n")
                       .arg(product.getName())
                       .arg(quantity)
                       .arg(product.getPrice() * quantity, 0, 'f', 2);
    }
    details += QString("Total Price (After Discount): %1").arg(totalPrice, 0, 'f', 2);
    return details;
}

// Get all products and their quantities in the cart
QList<Product> Cart::getProducts() const {
    QList<Product> products;
    for (auto it = productQuantities.begin(); it != productQuantities.end(); ++it) {
        int productId = it.key();
        Product product = Database::getProductById(productId);  // Fetch product details from the database using productId
        products.append(product);
    }
    return products;
}

// Get all products and their quantities in the cart as a map (for cart management)
QMap<int, int> Cart::getProductQuantities() const {
    return productQuantities;
}
