/**
 * @file manager.c
 * @brief Implements the Manager process to restock inventory and update prices.
 *
 * The Manager process allows the user to restock items in the shop and 
 * update item prices using shared memory synchronization.
 */

#include "manager.h"

/**
 * @brief Updates the shop inventory by restocking items or modifying prices.
 * 
 * This function provides a menu for the manager to:
 * - Restock an item by adding to its quantity.
 * - Update the price of an existing item.
 * - Exit the update menu.
 *
 * @param inventory Pointer to the shared inventory structure.
 */
void update_inventory(ShopInventory *inventory) {
    char item_name[MAX_ITEMS];
    int new_stock;
    float new_price;
    int choice;

    while (1) {
        printf("\n--- Manager Menu ---\n");
        printf("1. Restock an item\n");
        printf("2. Update item price\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 3) {
            break;
        }

        printf("Enter item name: ");
        scanf("%s", item_name);

        sem_wait(&inventory->mutex); // Lock inventory

        int found = 0;
        for (int i = 0; i < MAX_ITEMS; i++) {
            if (strcmp(inventory->items[i].name, item_name) == 0) {
                found = 1;
                if (choice == 1) {
                    printf("Enter quantity to add: ");
                    scanf("%d", &new_stock);
                    inventory->items[i].count += new_stock;
                    printf("✅ Restocked %s by %d units. New stock: %d\n", item_name, new_stock, inventory->items[i].count);
                } else if (choice == 2) {
                    printf("Enter new price: ");
                    scanf("%f", &new_price);
                    inventory->items[i].price = new_price;
                    printf("✅ Updated %s price to $%.2f\n", item_name, new_price);
                }
                break;
            }
        }

        if (!found) {
            printf("❌ Item not found in inventory.\n");
        }

        sem_post(&inventory->mutex); // Unlock inventory
    }
}

/**
 * @brief Entry point for the Manager process.
 * 
 * This function attaches to the shared memory segment containing
 * the inventory and starts the update process.
 *
 * @return int Exit status of the program.
 */
int main() {
    int shmid = shmget(SHM_KEY, sizeof(ShopInventory), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget failed");
        exit(1);
    }

    ShopInventory *inventory = (ShopInventory *)shmat(shmid, NULL, 0);
    if (inventory == (void *)-1) {
        perror("shmat failed");
        exit(1);
    }

    update_inventory(inventory);
    return 0;
}
