/**
 * \file slist.h
 * \brief Header for the s_list functions
 * \author Ayoub AOUNE ( SpartSystems )
 * \version 0.1
 * \date 01 octobre 2014
 *
 * This is a header file for the s_list functions, it contains all the prototypes of functions needed to develop an application using this list.
 *
 */


#ifndef SLIST_H
#define SLIST_H

typedef void (*slist_callback_print)(void *user_data);

/**
 * \struct s_list
 * \brief define a list.
 *
 * this list is used to define a list
 * - next : the next element of the list
 * - data : the data stored in the element of the list
 * - length : the length of the list.  
 *
 */
struct s_list {
	struct s_list *next;
	void *data;
	int length;
};


/**
 * \fn struct s_list *s_list_new();
 * \brief function used to initialize the slist.
 *
 * \param NONE
 * \return struct sloop_data.
 */
struct s_list *s_list_new();

/**
 * \fn int s_list_insert_at_position(struct s_list* list_d, int position,void * data);
 * \brief Insert an element in the list in a defined position
 *
 * \param list_d : the list.
 * \param position : the position where the element will be added. 
 * \param data : the data of this element. 
 * \return 0 if success , -1 if fail.
 */
int s_list_insert_at_position(struct s_list* list_d, int position,void * data);

/**
 * \fn int s_list_insert_last(struct s_list *list_d, void *item);
 * \brief Insert an element in the end of the list.
 *
 * \param list_d : the list.
 * \param data : the data of this element. 
 * \return 0 if success , -1 if fail.
 */
int s_list_insert_last(struct s_list *list_d, void *item);

/**
 * \fn int s_list_insert_first(struct s_list *list_d, void *item);
 * \brief Insert an element in the start of the list.
 *
 * \param list_d : the list.
 * \param data : the data of this element. 
 * \return 0 if success , -1 if fail.
 */
int s_list_insert_first(struct s_list *list_d, void *item);

/**
 * \fn int s_list_delete_item(struct s_list *list_d, void *item);
 * \brief Delete the element in the list which contains the item.
 *
 * \param list_d : the list.
 * \param data : the data of this element. 
 * \return 0 if success , -1 if fail.
 */
int s_list_delete_item(struct s_list *list_d, void *item);

/**
 * \fn int s_list_delete_at_position(struct s_list *list_d, int position);
 * \brief Delete an element in the list in a defined position
 *
 * \param list_d : the list.
 * \param position : the position where the element will be deleted.
 * \return 0 if success , -1 if fail.
 */
int s_list_delete_at_position(struct s_list *list_d, int position);

/**
 * \fn int s_list_delete_last(struct s_list *list_d);
 * \brief Delete the last element in the list.
 *
 * \param list_d : the list.
 * \return 0 if success , -1 if fail.
 */
int s_list_delete_last(struct s_list *list_d);

/**
 * \fn int s_list_destroy(struct s_list *list_d);
 * \brief Destroy the list.
 *
 * \param list_d : the list.
 * \return 0 if success , -1 if fail.
 */
int s_list_destroy(struct s_list *list_d);

/**
 * \fn int s_list_clean(struct s_list *list_d);
 * \brief clean the list.
 *
 * \param list_d : the list.
 * \return 0 if success , -1 if fail.
 */
int s_list_clean(struct s_list *list_d);

/**
 * \fn int s_list_print(struct s_list *list_d,slist_callback_print handler);
 * \brief print the list using the callback defined by the user.
 *
 * \param list_d : the list.
 * \param handler : the function to be called to print the value of the data.
 * \return 0 if success , -1 if fail.
 */
int s_list_print(struct s_list *list_d,slist_callback_print handler);

/**
 * \fn int s_list_get_position(struct s_list *list_d, void *item);
 * \brief get the position of the itme in the list.
 *
 * \param list_d : the list.
 * \param item : the data of this element. 
 * \return position of the item
 */
int s_list_get_position(struct s_list *list_d, void *item);

/**
 * \fn int s_list_get_length(struct s_list *list_d);
 * \brief get the length of the list.
 *
 * \param list_d : the list.
 * \return the length of the list.
 */
int s_list_get_length(struct s_list *list_d);

/**
 * \fn int s_list_is_empty(struct s_list *list_d);
 * \brief check if the list is empty.
 *
 * \param list_d : the list.
 * \return 1 if empty, 0 if not empty, -1 if error.
 */
int s_list_is_empty(struct s_list *list_d);

/**
 * \fn void * s_list_get_data_at_position(struct s_list *list_d,int position);
 * \brief get the data of the element in the position choosed by the user.
 *
 * \param list_d : the list.
 * \param position : the position where the element will be added.
 * \return return the data.
 */
void * s_list_get_data_at_position(struct s_list *list_d,int position);

#endif /* SLIST_H */