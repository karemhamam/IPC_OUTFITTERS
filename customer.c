/**
 * @file customer.c
 * @brief Customer process for interacting with the shop inventory.
 *
 * This program allows a customer to:
 * 1. View available items in the shop.
 * 2. Place an order while ensuring sufficient stock.
 * 3. Exit the ordering system.
 *
 * It uses shared memory and semaphores to synchronize access to inventory.
 */

#include "shop.h"

/**
 * @brief Handles customer actions such as viewing stock and placing orders.
 *
 * This function provides a menu-driven interface for the customer.
 * Customers can:
 * - View available items.
 * - Place an order while ensuring stock availability.
 * - Exit the system.
 *
 * @param inventory Pointer to shared memory shop inventory.
 */
void place_order(ShopInventory *inventory) {
    int num_items;
    char item_name[MAX_ITEMS];
    int quantity;

    while (1) {
        printf("\n--- Customer Menu ---\n");
        printf("1. View available items\n");
        printf("2. Place an order\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &num_items);

        if (num_items == 3) {
            break; ///< Exit if the user selects 3
        }

        sem_wait(&inventory->mutex); // Lock inventory

        if (num_items == 1) {
            /**
             * @brief Displays the available items in the shop.
             *
             * The function iterates through the inventory and prints
             * items that have stock greater than zero.
             */
            printf("\n📋 Available Items:\n");
            printf("-----------------------------\n");
            for (int i = 0; i < MAX_ITEMS; i++) {
                if (inventory->items[i].count > 0) {
                    printf("%s - %d in stock ($%.2f each)\n",
                           inventory->items[i].name,
                           inventory->items[i].count,
                           inventory->items[i].price);
                }
            }
            printf("-----------------------------\n");
        } else if (num_items == 2) {
            /**
             * @brief Handles customer order placement.
             *
             * Checks if the item exists and has sufficient stock before processing.
             */
            printf("Enter item name: ");
            scanf("%s", item_name);

            int found = 0;
            for (int i = 0; i < MAX_ITEMS; i++) {
                if (strcmp(inventory->items[i].name, item_name) == 0) {
                    found = 1;
                    printf("Enter quantity: ");
                    scanf("%d", &quantity);

                    if (quantity > inventory->items[i].count) {
                        printf("❌ Not enough stock available for %s. Only %d left.\n",
                               item_name, inventory->items[i].count);
                    } else {
                        inventory->items[i].count -= quantity;
                        printf("✅ Order placed: %d %s\n", quantity, item_name);
                    }
                    break;
                }
            }

            if (!found) {
                printf("❌ Item not found in inventory.\n");
            }
        }

        sem_post(&inventory->mutex); // Unlock inventory
    }
}

/**
 * @brief Main function for the customer process.
 *
 * - Attaches to shared memory.
 * - Calls `place_order` to interact with the inventory.
 * - Handles errors in shared memory attachment.
 *
 * @return 0 on successful execution.
 */
int main() {
    int shmid = shmget(SHM_KEY, sizeof(ShopInventory), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget failed"); ///< Error handling for shared memory
        exit(1);
    }

    ShopInventory *inventory = (ShopInventory *)shmat(shmid, NULL, 0);
    if (inventory == (void *)-1) {
        perror("shmat failed"); ///< Error handling for shared memory attachment
        exit(1);
    }

    place_order(inventory); ///< Start customer interaction
    return 0;
}
