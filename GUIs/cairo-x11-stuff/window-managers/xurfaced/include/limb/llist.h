struct limb_litem
{

    void *data;
    struct limb_litem *next;

};

struct limb_llist
{

    struct limb_litem *head;

};

struct limb_literator
{

    struct limb_llist *list;
    struct limb_litem *current;

};

extern struct limb_litem *limb_litem_create(void *data);
extern void limb_litem_destroy(struct limb_litem *item, void (*pre_callback)(void *data));

extern void limb_llist_add(struct limb_llist *list, struct limb_litem *item);
extern struct limb_llist *limb_llist_concat(struct limb_llist *list1, struct limb_llist *list2);
extern unsigned int limb_llist_count(struct limb_llist *list);
extern struct limb_llist *limb_llist_create();
extern void limb_llist_destroy(struct limb_llist *list);
extern void limb_llist_destroy_children(struct limb_llist *list, void (*pre_callback)(void *data));
extern void limb_llist_foreach(struct limb_llist *list, void (*callback)(void *data));
extern void limb_llist_remove(struct limb_llist *list, struct limb_litem *item);

extern struct limb_literator *limb_literator_create(struct limb_llist *list);
extern void limb_literator_destroy(struct limb_literator *iterator);
extern struct limb_litem *limb_literator_next(struct limb_literator *iterator);
extern void limb_literator_reset(struct limb_literator *iterator);

