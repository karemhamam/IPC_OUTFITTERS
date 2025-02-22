/**
 * @file shop.h
 * @brief Header file for the Shop process
 */

#ifndef SHOP_H
#define SHOP_H

#define MAX_ITEMS 10       ///< Maximum number of items in the shop
#define ITEM_NAME_LEN 20   ///< Maximum length of an item name

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/**
 * @struct Item
 * @brief Represents an item in the shop
 */
typedef struct {
    char name[ITEM_NAME_LEN]; ///< Name of the item
    int count;                ///< Available quantity of the item
    float price;              ///< Price of the item
} Item;

/**
 * @struct ShopInventory
 * @brief Shared memory structure for shop inventory
 */
typedef struct {
    Item items[MAX_ITEMS]; ///< Array of shop items
    sem_t mutex;           ///< Semaphore for synchronization
} ShopInventory;

/**
 * @struct Message
 * @brief Structure for inter-process communication messages
 */
typedef struct {
    long msg_type;              ///< Message type identifier
    char item_name[ITEM_NAME_LEN]; ///< Name of the item
    int quantity;               ///< Quantity of the item in the message
    float price;                ///< Price of the item in the message
} Message;

#define MSG_QUEUE_KEY 1234  ///< Message queue key for IPC
#define SHM_KEY 5678        ///< Shared memory key for IPC

#endif // SHOP_H
