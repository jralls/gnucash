#ifndef GMOCK_QOFBOOK_H
#define GMOCK_QOFBOOK_H

#include <gmock/gmock.h>

#include <qofbook.h>
#include <qofbook-p.h>

#include "gmock-gobject.h"
#include "gmock-Split.h"


GType qof_mock_book_get_type(void);

#define QOF_TYPE_MOCK_BOOK   (qof_mock_book_get_type ())
#define QOF_IS_MOCK_BOOK(o)  (G_TYPE_CHECK_INSTANCE_TYPE ((o), QOF_TYPE_MOCK_BOOK))


// mock up for QofBook
class QofMockBook : public QofBook
{
public:
    QofMockBook()
    {
        hash_of_collections   = nullptr;
        data_tables           = nullptr;
        data_table_finalizers = nullptr;

        book_open     = 'n';
        read_only     = TRUE;
        session_dirty = FALSE;

        version = 0;

        cached_num_field_source_isvalid      = FALSE;
        cached_num_days_autoreadonly_isvalid = FALSE;
    }
    void* operator new(size_t size)
    {
        return mock_g_object_new (QOF_TYPE_MOCK_BOOK, NULL, size);
    }

    // define separate free() function since destructor is protected
    void free()
    {
        delete this;
    }
    void operator delete(void* book, size_t size)
    {
        mock_g_object_unref(book, size);
    }

    MOCK_METHOD0(mallocSplit, Split *());
    MOCK_METHOD0(useSplitActionForNumField, gboolean());
    MOCK_CONST_METHOD2(foreach_collection, void(QofCollectionForeachCB, void*));
    MOCK_CONST_METHOD0(get_autoreadonly_gdate, GDate*());
    MOCK_CONST_METHOD0(get_backend, QofBackend*());
    MOCK_CONST_METHOD1(get_collection, QofCollection*(QofIdType));
    MOCK_CONST_METHOD1(get_data, void*(const char*));
    MOCK_CONST_METHOD0(is_readonly, gboolean());
    MOCK_METHOD0(mark_session_dirty, void());
    MOCK_METHOD2(set_data, void(const char*, void*));
    MOCK_CONST_METHOD0(shutting_down, gboolean());
    MOCK_CONST_METHOD0(use_trading_accounts, gboolean());
    MOCK_CONST_METHOD0(uses_autoreadonly, gboolean());
    MOCK_METHOD0(destroy, void());
    MOCK_CONST_METHOD0(empty, gboolean());
    MOCK_METHOD0(get_features, GHashTable*());
    MOCK_METHOD2(set_feature, void(const char*, const char*));
    MOCK_METHOD1(increment_and_format_counter, char*(const char *counter));
    MOCK_CONST_METHOD0(session_not_saved, gboolean());
    MOCK_METHOD1(set_backend, void(QofBackend* backend));
    MOCK_METHOD1(get_guid_option, const GncGUID*(GSList* path));
protected:

    // Protect destructor to avoid MockQofBook objects to be created on stack. MockQofBook
    // objects can only be dynamically created, since they are derived from GObject.
    ~QofMockBook() {}
};

#endif
