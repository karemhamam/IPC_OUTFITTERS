/**
 * @file shop.c
 * @brief Implementation of the shop process that manages inventory and processes customer orders.
 */

#include "shop.h"

/**
 * @brief Initializes the shop's inventory from a file.
 * @param inventory Pointer to the shop inventory shared memory structure.
 */
void initialize_inventory(ShopInventory *inventory) {
    FILE *file = fopen("inventory.txt", "r");
    if (!file) {
        perror("Failed to open inventory file");
        exit(1);
    }
    for (int i = 0; i < MAX_ITEMS; i++) {
        fscanf(file, "%s %d %f", inventory->items[i].name, &inventory->items[i].count, &inventory->items[i].price);
    }
    fclose(file);
}

/**
 * @brief Processes customer orders received through the message queue.
 * @param msgid Message queue identifier.
 * @param inventory Pointer to the shop inventory shared memory structure.
 */
void process_orders(int msgid, ShopInventory *inventory) {
    Message msg;
    while (msgrcv(msgid, &msg, sizeof(Message) - sizeof(long), 1, IPC_NOWAIT) > 0) {
        sem_wait(&inventory->mutex); // Lock inventory

        for (int i = 0; i < MAX_ITEMS; i++) {
            if (strcmp(inventory->items[i].name, msg.item_name) == 0) {
                if (inventory->items[i].count >= msg.quantity) {
                    inventory->items[i].count -= msg.quantity;
                    printf("Order processed: %d %s sold\n", msg.quantity, msg.item_name);
                } else {
                    printf("Insufficient stock for %s\n", msg.item_name);
                }
                break;
            }
        }

        sem_post(&inventory->mutex); // Unlock inventory
    }
}

/**
 * @brief Main function of the shop process.
 * Initializes shared memory, loads inventory, and continuously monitors orders and inventory status.
 * @return int Returns 0 on successful execution.
 */
int main() {
    // Create shared memory
    int shmid = shmget(SHM_KEY, sizeof(ShopInventory), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget failed");
        exit(1);
    }

    // Attach shared memory
    ShopInventory *inventory = (ShopInventory *)shmat(shmid, NULL, 0);
    if (inventory == (void *)-1) {
        perror("shmat failed");
        exit(1);
    }

    // Initialize semaphore
    sem_init(&inventory->mutex, 1, 1);

    // Load inventory data from file
    initialize_inventory(inventory);

    // Create message queue
    int msgid = msgget(MSG_QUEUE_KEY, IPC_CREAT | 0666);
    if (msgid == -1) {
        perror("msgget failed");
        exit(1);
    }
    
    const char *shop_name = 
    "\n"
    "██╗██████╗  ██████╗     ██████╗ ██╗   ██╗████████╗███████╗██╗████████╗████████╗███████╗██████╗ ███████╗\n"
    "██║██╔══██╗██╔════╝    ██╔═══██╗██║   ██║╚══██╔══╝██╔════╝██║╚══██╔══╝╚══██╔══╝██╔════╝██╔══██╗██╔════╝\n"
    "██║██████╔╝██║         ██║   ██║██║   ██║   ██║   █████╗  ██║   ██║      ██║   █████╗  ██████╔╝███████╗\n"
    "██║██╔═══╝ ██║         ██║   ██║██║   ██║   ██║   ██╔══╝  ██║   ██║      ██║   ██╔══╝  ██╔══██╗╚════██║\n"
    "██║██║     ╚██████╗    ╚██████╔╝╚██████╔╝   ██║   ██║     ██║   ██║      ██║   ███████╗██║  ██║███████║\n"
    "╚═╝╚═╝      ╚═════╝     ╚═════╝  ╚═════╝    ╚═╝   ╚═╝     ╚═╝   ╚═╝      ╚═╝   ╚══════╝╚═╝  ╚═╝╚══════╝\n";
    printf("%s", shop_name);

    // Periodically display inventory status and process orders
    while (1) {
        sleep(5);

        sem_wait(&inventory->mutex);
        printf("\n--- Inventory Status ---\n");
        for (int i = 0; i < MAX_ITEMS; i++) {
            printf("%s: %d units, $%.2f each\n", inventory->items[i].name, inventory->items[i].count, inventory->items[i].price);
        }
        sem_post(&inventory->mutex);

        process_orders(msgid, inventory);
    }

    return 0;
}
