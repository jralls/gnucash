#ifndef GMOCK_SPLIT_H
#define GMOCK_SPLIT_H

#include <gmock/gmock.h>
extern "C"
{
#include <Split.h>
#include <SplitP.h>
}
#include "gmock-qofbook.h"
#include "gmock-gobject.h"


GType gnc_mock_split_get_type(void);

#define GNC_TYPE_MOCK_SPLIT   (gnc_mock_split_get_type ())
#define GNC_IS_MOCK_SPLIT(o)  (G_TYPE_CHECK_INSTANCE_TYPE ((o), GNC_TYPE_MOCK_SPLIT))


// mock up for Split
class MockSplit : public Split
{
public:
    MockSplit()
    {
        acc         = nullptr;
        orig_acc    = nullptr;
        parent      = nullptr;
        orig_parent = nullptr;
        lot         = nullptr;

        action      = nullptr;
        memo        = nullptr;
        reconciled  = VREC;
        amount      = gnc_numeric_zero();
        value       = gnc_numeric_zero();

        date_reconciled    = 0;

        balance            = gnc_numeric_zero();
        cleared_balance    = gnc_numeric_zero();
        reconciled_balance = gnc_numeric_zero();
        noclosing_balance  = gnc_numeric_zero();

        gains        = GAINS_STATUS_UNKNOWN;
        gains_split  = nullptr;
    }
    void* operator new(size_t size)
    {
        return mock_g_object_new (GNC_TYPE_MOCK_SPLIT, NULL, size);
    }

    // define separate free() function since destructor is protected
    void free()
    {
        delete this;
    }
    void operator delete(void* split, size_t size)
    {
        mock_g_object_unref(split, size);
    }

    MOCK_METHOD0(init, void());
    MOCK_METHOD0(getBook, QofBook *());
    MOCK_METHOD0(getAccount, Account *());
    MOCK_METHOD1(setAccount, void(Account*));
    MOCK_METHOD0(getAmount, gnc_numeric());
    MOCK_METHOD1(setAmount, void(gnc_numeric));
    MOCK_METHOD0(getValue, gnc_numeric());
    MOCK_METHOD1(setValue, void(gnc_numeric));
    MOCK_METHOD0(getMemo, const char *());
    MOCK_METHOD0(getReconcile, char());
    MOCK_METHOD1(setReconcile, void(char));
    MOCK_METHOD1(setDateReconciledSecs, void(time64));
    MOCK_METHOD0(getAction, const char *());
    MOCK_METHOD1(setAction, void(const char*));
    MOCK_METHOD0(getOtherSplit, Split *());
    MOCK_METHOD0(getParent, Transaction *());
    MOCK_METHOD1(setParent, void(Transaction*));

    MOCK_METHOD0(Destroy, gboolean());
    MOCK_METHOD0(CommitEdit, void());
    MOCK_METHOD0(RollbackEdit, void() );
    MOCK_CONST_METHOD4(Equal, gboolean(const Split*, gboolean, gboolean, gboolean));
    MOCK_CONST_METHOD1(Order, int(const Split*));
    MOCK_CONST_METHOD1(OrderDateOnly, int(const Split*));
    MOCK_METHOD0(Unvoid, void());
    MOCK_METHOD0(Void, void());
    MOCK_METHOD0(DetermineGainStatus, void());
    MOCK_METHOD0(Free, void());
    MOCK_CONST_METHOD0(Dupe, Split*());
    MOCK_CONST_METHOD0(CloneNoKvp, Split*());
    MOCK_CONST_METHOD1(CopyKvp, void(Split*));
    MOCK_CONST_METHOD0(GetBalance, gnc_numeric());
    MOCK_METHOD1(SetMemo, void(const char*));
    MOCK_CONST_METHOD0(GetType, char*());
    MOCK_CONST_METHOD0(GetNoclosingBalance, gnc_numeric());
    MOCK_METHOD0(DateReconciled, time64());
    MOCK_METHOD2(AddPeerSplit, void(const Split*, const time64));
    MOCK_CONST_METHOD1(CopyOnto, void(Split*));
    MOCK_CONST_METHOD0(GetDateReconciled, time64());
    MOCK_CONST_METHOD0(GetLot, GNCLot*());
    MOCK_CONST_METHOD0(HasPeers, gboolean());
    MOCK_CONST_METHOD1(IsPeerSplit, gboolean(const Split*));
    MOCK_METHOD1(MergePeerSplits, void(const Split*));
    MOCK_METHOD1(RemovePeerSplit, void(const Split*));
    MOCK_METHOD2(SetBaseValue, void(gnc_numeric, const gnc_commodity*));
    MOCK_METHOD1(SetLot, void(GNCLot*));
    MOCK_METHOD1(SetSharePrice, void(gnc_numeric));
    MOCK_METHOD0(MarkDirty, void());

protected:
    // Protect destructor to avoid MockSplit objects to be created on stack. MockSplit
    // objects can only be dynamically created, since they are derived from GObject.
    ~MockSplit() {}
};

#endif
