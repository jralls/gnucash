#include "gmock-qofbook.h"

struct _QofMockBookClass
{
    QofInstanceClass parent_class;
};
typedef struct _QofMockBookClass QofMockBookClass;

G_DEFINE_TYPE(QofMockBook, qof_mock_book, QOF_TYPE_INSTANCE);

extern "C"
{
static void
qof_mock_book_init (QofMockBook *inst)
{
    // function is unused, initialization is done in the QofMockBook's constructor
}

static void
qof_mock_book_class_init(QofMockBookClass *klass)
{
    // function is unused, class functions are defined in C++ code
}

gboolean
qof_book_use_split_action_for_num_field (const QofBook *book)
{
    g_return_val_if_fail(QOF_IS_MOCK_BOOK(book), FALSE);
    return ((QofMockBook*)book)->useSplitActionForNumField();
}

static inline QofMockBook*
mock_book(QofBook* book)
{
    return static_cast<QofMockBook*>(book);
}

static inline const QofMockBook*
mock_book(const QofBook* book)
{
    return static_cast<const QofMockBook*>(book);
}

void
qof_book_foreach_collection(const QofBook* book, QofCollectionForeachCB thunk,
                            void* data)
{
    mock_book(book)->foreach_collection(thunk, data);
}

GDate*
qof_book_get_autoreadonly_gdate(const QofBook* book)
{
    return mock_book(book)->get_autoreadonly_gdate();
}

QofBackend*
qof_book_get_backend(const QofBook* book)
{
    return mock_book(book)->get_backend();
}

QofCollection*
qof_book_get_collection(const QofBook* book, QofIdType etype)
{
    return mock_book(book)->get_collection(etype);
}

void*
qof_book_get_data(const QofBook* book, const char* key)
{
    return mock_book(book)->get_data(key);
}

GType
qof_book_get_type(void)
{
    return qof_mock_book_get_type();
}

gboolean
qof_book_is_readonly(const QofBook* book)
{
    return mock_book(book)->is_readonly();
}

void
qof_book_mark_session_dirty(QofBook* book)
{
    mock_book(book)->mark_session_dirty();
}

void
qof_book_set_data(QofBook* book, const char* key, void* data)
{
    mock_book(book)->set_data(key, data);
}

gboolean
qof_book_shutting_down(const QofBook* book)
{
    return mock_book(book)->shutting_down();
}

gboolean
qof_book_use_trading_accounts(const QofBook* book)
{
    return mock_book(book)->use_trading_accounts();
}

gboolean
qof_book_uses_autoreadonly(const QofBook* book)
{
    return mock_book(book)->uses_autoreadonly();
}

void
qof_book_destroy(QofBook *book)
{
    mock_book(book)->destroy();
}

gboolean
qof_book_empty(const QofBook *book)
{
    return mock_book(book)->empty();
}

GHashTable*
qof_book_get_features(QofBook *book)
{
    return mock_book(book)->get_features();
}

void
qof_book_set_feature(QofBook *book, const char* key, const char* desc)
{
    mock_book(book)->set_feature(key, desc);
}

char*
qof_book_increment_and_format_counter(QofBook *book, const char *counter)
{
    return mock_book(book)->increment_and_format_counter(counter);
}

QofBook*
qof_book_new(void)
{
    return new QofMockBook;
}

gboolean
qof_book_register(void)
{
    return TRUE;
}

gboolean
qof_book_session_not_saved(const QofBook* book)
{
    return mock_book(book)->session_not_saved();
}

void
qof_book_set_backend(QofBook* book, QofBackend* backend)
{
    mock_book(book)->set_backend(backend);
}

const GncGUID*
qof_book_get_guid_option(QofBook* book, GSList* path)
{
    return mock_book(book)->get_guid_option(path);
}

} // extern "C"
