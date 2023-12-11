struct limb_ditem
{

    void *data;
    struct limb_ditem *next;
    struct limb_ditem *previous;

};

struct limb_dlist
{

    struct limb_ditem *head;

};

struct limb_diterator
{

    struct limb_dlist *list;
    struct limb_ditem *current;

};

extern struct limb_ditem *limb_ditem_create(void *data);
extern void limb_ditem_destroy(struct limb_ditem *item, void (*pre_callback)(void *data));

extern void limb_dlist_add(struct limb_dlist *list, struct limb_ditem *item);
extern struct limb_dlist *limb_dlist_concat(struct limb_dlist *list1, struct limb_dlist *list2);
extern unsigned int limb_dlist_count(struct limb_dlist *list);
extern struct limb_dlist *limb_dlist_create();
extern void limb_dlist_destroy(struct limb_dlist *list);
extern void limb_dlist_destroy_children(struct limb_dlist *list, void (*pre_callback)(void *data));
extern void limb_dlist_foreach(struct limb_dlist *list, void (*callback)(void *data));
extern void limb_dlist_remove(struct limb_dlist *list, struct limb_ditem *item);

extern struct limb_diterator *limb_diterator_create(struct limb_dlist *list);
extern void limb_diterator_destroy(struct limb_diterator *iterator);
extern struct limb_ditem *limb_diterator_next(struct limb_diterator *iterator);
extern void limb_diterator_reset(struct limb_diterator *iterator);

