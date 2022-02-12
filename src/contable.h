#ifndef CON_TABLE_H_
#define CON_TABLE_H_

#ifndef NULL
#define NULL    ((void *)0)
#endif


// How to represent the data in the column
typedef enum
{
  contable_decimal,
  contable_hex,
  contable_string,
} con_col_type;

// Link list node that will be allocated for each column of our table.
typedef struct con_col_t
{
  char             name[32];
  int              width;
  con_col_type     type;
  struct con_col_t *next;   // Next linked element.
} con_col;


// Table object associated with each unique table we draw.
typedef struct con_table_t
{
  bool    columnsAdded;
  int     totalWidh;
  con_col *columns;   // The head of our columns list.
} con_table;

/**************************************************************************/ /**
 * @brief
 *  Add a column to the table.
 * @return bool.
 *
 *****************************************************************************/

bool contable_add_column(con_table *tbl, char *name, con_col_type type, int width);

/**************************************************************************/ /**
 * @brief
 *  Print a table raw separator.
 * @return void.
 *
 *****************************************************************************/

void contable_print_separator(con_table *tbl);

/**************************************************************************/ /**
 * @brief
 *   Adds a blank line to the table.
 * @return void.
 *
 *****************************************************************************/

void contable_print_blank(con_table *tbl);

/**************************************************************************/ /**
 * @brief
 *  Print table raw, 'printf' style.
 *  Eeach '%' arg will be positioned accroding to the columns we've added.
 *  The first call to this function blocks the option to add more columns.
 * @return void.
 *
 *****************************************************************************/

int contable_print(con_table *tbl, const char *_format, ...);

/**************************************************************************/ /**
 * @brief
 *  Destroy a table object and free it's internal columns list.
 * @return void.
 *
 *****************************************************************************/

void contable_destroy(con_table *tbl, bool separator);

/**************************************************************************/ /**
 * @brief
 *  Initializes a new table object.
 * @return void.
 *
 *****************************************************************************/

void contable_init(con_table *tbl);

#endif // CON_TABLE_H_
