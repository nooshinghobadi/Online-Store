#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QList>
#include "product.h"
#include "order.h"

class Database {
public:
    // Static method to initialize the database
    static bool initialize();

    // CRUD operations for products
    static bool addProduct(const Product &product);
    static QList<Product> getAllProducts();
    static Product getProductById(int productId); // Added this method to fetch a product by its ID
    static bool updateProduct(const Product &product);
    static bool deleteProduct(int productId);

    // CRUD operations for orders
    static bool addOrder(const Order &order);  // Handles order creation with products and quantities
    static QList<Order> getAllOrders();       // Returns orders along with associated products and quantities
};

#endif // DATABASE_H
