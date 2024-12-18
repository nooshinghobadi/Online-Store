#include "database.h"
#include "product.h"
#include "order.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

bool Database::initialize() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("store.db");  // Path to your SQLite database file

    if (!db.open()) {
        qDebug() << "Error: Unable to connect to database" << db.lastError();
        return false;
    }

    QSqlQuery query;

    // Create Products table
    if (!query.exec("CREATE TABLE IF NOT EXISTS Products ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                    "name TEXT, "
                    "price REAL, "
                    "stock INTEGER, "
                    "description TEXT, "
                    "discount REAL DEFAULT 0)")) {
        qDebug() << "Error creating Products table:" << query.lastError();
        return false;
    }

    // Create Orders table
    if (!query.exec("CREATE TABLE IF NOT EXISTS Orders ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                    "date TEXT, "
                    "total_price REAL, "
                    "status TEXT)")) {
        qDebug() << "Error creating Orders table:" << query.lastError();
        return false;
    }

    // Create OrderProducts table
    if (!query.exec("CREATE TABLE IF NOT EXISTS OrderProducts ("
                    "order_id INTEGER, "
                    "product_id INTEGER, "
                    "quantity INTEGER, "
                    "FOREIGN KEY(order_id) REFERENCES Orders(id), "
                    "FOREIGN KEY(product_id) REFERENCES Products(id))")) {
        qDebug() << "Error creating OrderProducts table:" << query.lastError();
        return false;
    }

    return true;
}

bool Database::addProduct(const Product &product) {
    QSqlQuery query;
    query.prepare("INSERT INTO Products (name, price, stock, description, discount) "
                  "VALUES (:name, :price, :stock, :description, :discount)");

    query.bindValue(":name", product.getName());
    query.bindValue(":price", product.getPrice());
    query.bindValue(":stock", product.getStock());
    query.bindValue(":description", product.getDescription());
    query.bindValue(":discount", product.getDiscount());

    if (!query.exec()) {
        qDebug() << "Error adding product:" << query.lastError();
        return false;
    }

    return true;
}

QList<Product> Database::getAllProducts() {
    QList<Product> products;
    QSqlQuery query("SELECT id, name, price, stock, description, discount FROM Products");

    if (!query.exec()) {
        qDebug() << "Error fetching products:" << query.lastError();
        return products;
    }

    while (query.next()) {
        products.append(Product(
            query.value(0).toInt(),    // id
            query.value(1).toString(), // name
            query.value(2).toDouble(), // price
            query.value(3).toInt(),    // stock
            query.value(4).toString(), // description
            query.value(5).toDouble()  // discount
        ));
    }

    return products;
}

Product Database::getProductById(int productId) {
    QSqlQuery query;
    query.prepare("SELECT id, name, price, stock, description, discount FROM Products WHERE id = :id");
    query.bindValue(":id", productId);

    if (!query.exec()) {
        qDebug() << "Error fetching product by ID:" << query.lastError();
        return Product(); // Return an empty product if the query fails
    }

    if (query.next()) {
        return Product(
            query.value(0).toInt(),    // id
            query.value(1).toString(), // name
            query.value(2).toDouble(), // price
            query.value(3).toInt(),    // stock
            query.value(4).toString(), // description
            query.value(5).toDouble()  // discount
        );
    }

    return Product(); // Return an empty product if no product is found
}

bool Database::updateProduct(const Product &product) {
    QSqlQuery query;
    query.prepare("UPDATE Products SET name = :name, price = :price, stock = :stock, description = :description, discount = :discount "
                  "WHERE id = :id");

    query.bindValue(":id", product.getId());
    query.bindValue(":name", product.getName());
    query.bindValue(":price", product.getPrice());
    query.bindValue(":stock", product.getStock());
    query.bindValue(":description", product.getDescription());
    query.bindValue(":discount", product.getDiscount());

    if (!query.exec()) {
        qDebug() << "Error updating product:" << query.lastError();
        return false;
    }

    return true;
}

bool Database::deleteProduct(int productId) {
    QSqlQuery query;
    query.prepare("DELETE FROM Products WHERE id = :id");
    query.bindValue(":id", productId);

    if (!query.exec()) {
        qDebug() << "Error deleting product:" << query.lastError();
        return false;
    }

    return true;
}

bool Database::addOrder(const Order &order) {
    QSqlQuery query;
    query.prepare("INSERT INTO Orders (date, total_price, status) "
                  "VALUES (:date, :total_price, :status)");

    query.bindValue(":date", order.getDate());
    query.bindValue(":total_price", order.getTotalPrice());
    query.bindValue(":status", order.getStatus());

    if (!query.exec()) {
        qDebug() << "Error adding order:" << query.lastError();
        return false;
    }

    int orderId = query.lastInsertId().toInt();  // Get the ID of the newly inserted order

    // Link products and their quantities to the order
    for (int i = 0; i < order.getProducts().size(); ++i) {
        const Product product = order.getProducts()[i];
        int quantity = order.getQuantities()[i];

        query.prepare("INSERT INTO OrderProducts (order_id, product_id, quantity) "
                      "VALUES (:order_id, :product_id, :quantity)");
        query.bindValue(":order_id", orderId);
        query.bindValue(":product_id", product.getId());
        query.bindValue(":quantity", quantity);

        if (!query.exec()) {
            qDebug() << "Error linking product to order:" << query.lastError();
            return false;
        }
    }

    return true;
}

QList<Order> Database::getAllOrders() {
    QList<Order> orders;
    QSqlQuery query("SELECT id, date, total_price, status FROM Orders");

    if (!query.exec()) {
        qDebug() << "Error fetching orders:" << query.lastError();
        return orders;
    }

    while (query.next()) {
        int id = query.value(0).toInt();
        QString date = query.value(1).toString();
        double totalPrice = query.value(2).toDouble();
        QString status = query.value(3).toString();

        // Fetch products and quantities for this order
        QList<Product> products;
        QList<int> quantities;

        QSqlQuery productQuery;
        productQuery.prepare("SELECT Products.id, name, price, stock, description, discount, OrderProducts.quantity "
                             "FROM Products "
                             "INNER JOIN OrderProducts ON Products.id = OrderProducts.product_id "
                             "WHERE OrderProducts.order_id = :order_id");
        productQuery.bindValue(":order_id", id);
        productQuery.exec();

        while (productQuery.next()) {
            products.append(Product(
                productQuery.value(0).toInt(),    // id
                productQuery.value(1).toString(), // name
                productQuery.value(2).toDouble(), // price
                productQuery.value(3).toInt(),    // stock
                productQuery.value(4).toString(), // description
                productQuery.value(5).toDouble()  // discount
            ));
            quantities.append(productQuery.value(6).toInt()); // quantity
        }

        orders.append(Order(id, date, products, quantities, totalPrice, status));
    }

    return orders;
}
