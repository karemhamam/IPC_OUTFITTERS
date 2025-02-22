# IPC Outfitters

IPC Outfitters is a multi-process inventory management system using Inter-Process Communication (IPC) mechanisms such as shared memory, message queues, and semaphores. The system consists of three main processes:

- **Shop Process**: Manages inventory and processes customer orders.
- **Customer Process**: Places orders by interacting with the shop.
- **Manager Process**: Updates stock and modifies prices.

## Features
- Customers can place multiple item orders.
- The shop updates inventory based on customer purchases.
- The manager can restock and update prices.
- Synchronization is ensured using semaphores.

## Installation & Compilation
### 1. Clone the repository
```sh
 git clone <repo_link>
 cd IPC_Outfitters
```

### 2. Compile the programs
```sh
gcc shop.c -o shop -pthread -lrt
gcc customer.c -o customer -pthread -lrt
gcc manager.c -o manager -pthread -lrt
```

## Usage
1. **Start the Shop Process:**
   ```sh
   ./shop
   ```
2. **Run the Manager Process to update inventory:**
   ```sh
   ./manager
   ```
3. **Run the Customer Process to place orders:**
   ```sh
   ./customer
   ```

## Testing & Expected Output
- If a customer orders an available item, the stock is reduced, and an order confirmation is printed.
- If an item is out of stock, the system displays an error.
- The manager can restock and update prices dynamically.

### Example Run
#### Shop:
```
--- Inventory Status ---
Shirt: 10 units, $20.00 each
Pants: 5 units, $35.00 each
```

#### Customer:
```
Enter the number of different items you want to order: 1
Enter item 1 name: Shirt
Enter quantity for Shirt: 2
✅ Order placed: 2 Shirt
```

#### Shop (after order):
```
--- Inventory Status ---
Shirt: 8 units, $20.00 each
Pants: 5 units, $35.00 each
```

#### Manager (restocking):
```
1. Restock an item
2. Update item price
3. Exit
Enter your choice: 1
Enter item name: Shirt
Enter quantity to add: 5
✅ Restocked Shirt by 5 units. New stock: 13
```

## Documentation
This project is documented using Doxygen. To generate documentation:
```sh
doxygen Doxyfile
```

## How I Designed it

![Image](https://github.com/user-attachments/assets/c0aca435-5dc1-4130-9e3a-2b2978d990ca)
