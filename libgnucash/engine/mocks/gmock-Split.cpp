#include <config.h>

#include "gmock-Split.h"
#include "gmock-qofbook.h"
#include "gmock-Account.h"
#include "gmock-Transaction.h"


struct _MockSplitClass
{
    QofInstanceClass parent_class;
};
typedef struct _MockSplitClass MockSplitClass;

G_DEFINE_TYPE(MockSplit, gnc_mock_split, QOF_TYPE_INSTANCE);

static void
gnc_mock_split_init (MockSplit *inst)
{
    // function is unused since it's overwritten by MockSplit's constructor anyway
}

static void
gnc_mock_split_class_init (MockSplitClass *klass)
{
    // function is unused, class functions are defined in C++ code
}

extern "C"
{
gboolean
xaccSplitRegister ()
{
    return TRUE;
}

GType gnc_split_get_type(void)
{
    return gnc_mock_split_get_type();
}

Split *
xaccMallocSplit (QofBook *book)
{
    g_return_val_if_fail(QOF_IS_MOCK_BOOK(book), NULL);
    return ((QofMockBook*)book)->mallocSplit();
}

QofBook *
xaccSplitGetBook (const Split *split)
{
    g_return_val_if_fail(GNC_IS_MOCK_SPLIT(split), NULL);
    return ((MockSplit*)split)->getBook();
}

Account *
xaccSplitGetAccount (const Split *split)
{
    g_return_val_if_fail(GNC_IS_MOCK_SPLIT(split), NULL);
    return ((MockSplit*)split)->getAccount();
}

void
xaccSplitSetAccount (Split *split, Account *acc)
{
    g_return_if_fail(GNC_IS_MOCK_SPLIT(split));
    g_return_if_fail(GNC_IS_ACCOUNT(acc));
    ((MockSplit*)split)->setAccount(acc);
}

gnc_numeric
xaccSplitGetAmount (const Split *split)
{
    g_return_val_if_fail(GNC_IS_MOCK_SPLIT(split), gnc_numeric_zero());
    return ((MockSplit*)split)->getAmount();
}

void
xaccSplitSetAmount (Split *split, gnc_numeric amt)
{
    g_return_if_fail(GNC_IS_MOCK_SPLIT(split));
    ((MockSplit*)split)->setAmount(amt);
}

gnc_numeric
xaccSplitGetValue (const Split *split)
{
    g_return_val_if_fail(GNC_IS_MOCK_SPLIT(split), gnc_numeric_zero());
    return ((MockSplit*)split)->getValue();
}

void
xaccSplitSetValue (Split *split, gnc_numeric val)
{
    g_return_if_fail(GNC_IS_MOCK_SPLIT(split));
    ((MockSplit*)split)->setValue(val);
}

const char *
xaccSplitGetMemo (const Split *split)
{
    g_return_val_if_fail(GNC_IS_MOCK_SPLIT(split), NULL);
    return ((MockSplit*)split)->getMemo();
}

char
xaccSplitGetReconcile (const Split *split)
{
    g_return_val_if_fail(GNC_IS_MOCK_SPLIT(split), VREC);
    return ((MockSplit*)split)->getReconcile();
}

void
xaccSplitSetReconcile (Split *split, char recn)
{
    g_return_if_fail(GNC_IS_MOCK_SPLIT(split));
    ((MockSplit*)split)->setReconcile(recn);
}

void
xaccSplitSetDateReconciledSecs (Split *split, time64 secs)
{
    g_return_if_fail(GNC_IS_MOCK_SPLIT(split));
    ((MockSplit*)split)->setDateReconciledSecs(secs);
}

const char *
xaccSplitGetAction (const Split *split)
{
    g_return_val_if_fail(GNC_IS_MOCK_SPLIT(split), NULL);
    return ((MockSplit*)split)->getAction();
}

void
xaccSplitSetAction (Split *split, const char* action)
{
    g_return_if_fail(GNC_IS_MOCK_SPLIT(split));
    ((MockSplit*)split)->setAction(action);
}

Split *
xaccSplitGetOtherSplit (const Split *split)
{
    g_return_val_if_fail(GNC_IS_MOCK_SPLIT(split), NULL);
    return ((MockSplit*)split)->getOtherSplit();
}

Transaction *
xaccSplitGetParent (const Split *split)
{
    g_return_val_if_fail(GNC_IS_MOCK_SPLIT(split), NULL);
    return ((MockSplit*)split)->getParent();
}

void
xaccSplitSetParent(Split *split, Transaction *trans)
{
    g_return_if_fail(GNC_IS_MOCK_SPLIT(split));
    g_return_if_fail(GNC_IS_TRANSACTION(trans));
    ((MockSplit*)split)->setParent(trans);
}

static inline MockSplit*
mock_split(Split* split)
{
    return static_cast<MockSplit*>(split);
}

static inline const MockSplit*
mock_split(const Split* split)
{
    return static_cast<const MockSplit*>(split);
}

gboolean
xaccSplitDestroy(Split *split)
{
    return mock_split(split)->Destroy();
}

void
xaccSplitCommitEdit(Split* split)
{
    mock_split(split)->CommitEdit();
}

void
xaccSplitRollbackEdit(Split* split)
{
    mock_split(split)->RollbackEdit();
}

gboolean
xaccSplitEqual(const Split* ls, const Split* rs, gboolean check_guids,
               gboolean check_balances, gboolean check_txn_splits)
{
    return mock_split(ls)->Equal(rs, check_guids, check_balances,
                                 check_txn_splits);
}

int
xaccSplitOrder(const Split* ls, const Split* rs)
{
    return mock_split(ls)->Order(rs);
}

int
xaccSplitOrderDateOnly(const Split* ls, const Split* rs)
{
    return mock_split(ls)->OrderDateOnly(rs);
}

void
xaccSplitVoid(Split* split)
{
    mock_split(split)->Void();
}

void
xaccSplitUnvoid(Split* split)
{
    mock_split(split)->Unvoid();
}

void
xaccSplitDetermineGainStatus(Split* split)
{
    mock_split(split)->DetermineGainStatus();
}

void
xaccFreeSplit(Split* split)
{
    mock_split(split)->Free();
}

Split*
xaccDupeSplit(const Split* split)
{
    return mock_split(split)->Dupe();
}

Split*
xaccSplitCloneNoKvp(const Split* split)
{
    return mock_split(split)->CloneNoKvp();
}

void
xaccSplitCopyKvp(const Split* from, Split* to)
{
    mock_split(from)->CopyKvp(to);
}

gnc_numeric
xaccSplitGetBalance(const Split* split)
{
    return mock_split(split)->GetBalance();
}

void
xaccSplitSetMemo(Split* split, const char* memo)
{
    mock_split(split)->SetMemo(memo);
}

const char*
xaccSplitGetType(const Split* split)
{
    return mock_split(split)->GetType();
}

gnc_numeric
xaccSplitGetNoclosingBalance(const Split* split)
{
    return mock_split(split)->GetNoclosingBalance();
}

time64
xaccSplitDateReconciled(Split* split)
{
    return mock_split(split)->DateReconciled();
}

void
xaccSplitAddPeerSplit(Split *split, const Split *other_split, const time64 timestamp)
{
    mock_split(split)->AddPeerSplit(other_split, timestamp);
}

void xaccSplitCopyOnto(const Split *from_split, Split *to_split)
{
    mock_split(from_split)->CopyOnto(to_split);
}

time64 xaccSplitGetDateReconciled(const Split *split)
{
    return mock_split(split)->GetDateReconciled();
}

GNCLot* xaccSplitGetLot(const Split *split)
{
    return mock_split(split)->GetLot();
}

gboolean xaccSplitHasPeers(const Split *split)
{
    return mock_split(split)->HasPeers();
}

gboolean xaccSplitIsPeerSplit(const Split *split, const Split *other_split)
{
    return mock_split(split)->IsPeerSplit(other_split);
}

void xaccSplitMergePeerSplits(Split *split, const Split *other_split)
{
    mock_split(split)->MergePeerSplits(other_split);
}

void xaccSplitRemovePeerSplit(Split *split, const Split *other_split)
{
    mock_split(split)->RemovePeerSplit(other_split);
}

void xaccSplitSetBaseValue(Split *split, gnc_numeric value,
                                    const gnc_commodity * base_currency)
{
    mock_split(split)->SetBaseValue(value, base_currency);
}

void xaccSplitSetLot(Split* split, GNCLot* lot)
{
    mock_split(split)->SetLot(lot);
}

void xaccSplitSetSharePrice(Split *split, gnc_numeric price)
{
    mock_split(split)->SetSharePrice(price);
}

void mark_split(Split *split)
{
    mock_split(split)->MarkDirty();
}

} //extern "C"
