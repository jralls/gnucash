/********************************************************************\
 * gnc-option.hpp -- Application options system                     *
 * Copyright (C) 2019 John Ralls <jralls@ceridwen.us>               *
 *                                                                  *
 * This program is free software; you can redistribute it and/or    *
 * modify it under the terms of the GNU General Public License as   *
 * published by the Free Software Foundation; either version 2 of   *
 * the License, or (at your option) any later version.              *
 *                                                                  *
 * This program is distributed in the hope that it will be useful,  *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of   *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the    *
 * GNU General Public License for more details.                     *
 *                                                                  *
 * You should have received a copy of the GNU General Public License*
 * along with this program; if not, contact:                        *
 *                                                                  *
 * Free Software Foundation           Voice:  +1-617-542-5942       *
 * 51 Franklin Street, Fifth Floor    Fax:    +1-617-542-2652       *
 * Boston, MA  02110-1301,  USA       gnu@gnu.org                   *
 *                                                                  *
\********************************************************************/

#ifndef GNC_OPTION_HPP_
#define GNC_OPTION_HPP_

extern "C"
{
#include <config.h>
#include <qof.h>
#include <Account.h>
#include <gnc-budget.h>
#include <gnc-commodity.h>
}
#include <gnc-datetime.hpp>
#include <libguile.h>
#include <string>
#include <utility>
#include <vector>
#include <exception>
#include <functional>
#include <variant>
#include <iostream>

/*
 * Unused base class to document the structure of the current Scheme option
 * vector, re-expressed in C++. The comment-numbers on the right indicate which
 * item in the Scheme vector each item implements.
 *
 * Not everything here needs to be implemented, nor will it necessarily be
 * implemented the same way. For example, part of the purpose of this redesign
 * is to convert from saving options as strings of Scheme code to some form of
 * key-value pair in the book options, so generate_restore_form() will likely be
 * supplanted with save_to_book().

template <typename ValueType>
class GncOptionBase
{
public:
    virtual ~GncOption = default;
    virtual ValueType get_value() const = 0;                             //5
    virtual ValueType get_default_value() = 0;
    virtual SCM get_SCM_value() = 0;
    virtual SCM get_SCM_default_value() const = 0;                       //7
    virtual void set_value(ValueType) = 0;                               //6
// generate_restore_form outputs a Scheme expression (a "form") that finds an
// option and sets it to the current value. e.g.:
//(let ((option (gnc:lookup-option options
//                                 "Display"
//                                 "Amount")))
//  ((lambda (option) (if option ((gnc:option-setter option) 'none))) option))
// it uses gnc:value->string to generate the "'none" (or whatever the option's
// value would be as input to the scheme interpreter).

    virtual std::string generate_restore_form();                         //8
    virtual void save_to_book(QofBook*) const noexcept;                  //9
    virtual void read_from_book(QofBook*);                               //10
    virtual std::vector<std::string> get_option_strings();               //15
    virtual set_changed_callback(std::function<void(void*)>);            //14
protected:
    const std::string m_section;                                         //0
    const std::string m_name;                                            //1
    const std::string m_sort_tag;                                        //2
    const std::type_info m_kvp_type;                                     //3
    const std::string m_doc_string;                                      //4
    std::function<void(void*)> m_changed_callback;   //Part of the make-option closure
    std::function<void(void*)>m_option_widget_changed_callback;          //16
};
*/

enum GncOptionUIType
{
    INTERNAL,
    BOOLEAN,
    STRING,
    TEXT,
    CURRENCY,
    COMMODITY,
    MULTICHOICE,
    DATE,
    ACCOUNT_LIST,
    ACCOUNT_SEL,
    LIST,
    NUMBER_RANGE,
    COLOR,
    FONT,
    BUDGET,
    PIXMAP,
    RADIOBUTTON,
    DATE_FORMAT,
    OWNER,
    CUSTOMER,
    VENDOR,
    EMPLOYEE,
    INVOICE,
    TAX_TABLE,
    QUERY,
};

struct OptionClassifier
{
    std::string m_section;
    std::string m_name;
    std::string m_sort_tag;
//  std::type_info m_kvp_type;
    std::string m_doc_string;
};

class GncOptionUIItem;

/**
 * Holds a pointer to the UI item which will control the option and an enum
 * representing the type of the option for dispatch purposes; all of that
 * happens in gnucash/gnome-utils/dialog-options and
 * gnucash/gnome/business-option-gnome.
 *
 * This class takes no ownership responsibility, so calling code is responsible
 * for ensuring that the UI_Item is alive. For convenience the public
 * clear_ui_item function can be used as a weak_ptr's destruction callback to
 * ensure that the ptr will be nulled if the ui_item is destroyed elsewhere.
 */
class OptionUIItem
{
public:
    GncOptionUIType get_ui_type() const { return m_ui_type; }
    GncOptionUIItem* const get_ui_item() const {return m_ui_item; }
    void clear_ui_item() { m_ui_item = nullptr; }
    void set_ui_item(GncOptionUIItem* ui_item)
    {
        if (m_ui_type == GncOptionUIType::INTERNAL)
        {
            std::string error{"INTERNAL option, setting the UI item forbidden."};
            throw std::logic_error(std::move(error));
        }
        m_ui_item = ui_item;
    }
    void make_internal()
    {
        if (m_ui_item != nullptr)
        {
            std::string error("Option has a UI Element, can't be INTERNAL.");
            throw std::logic_error(std::move(error));
        }
        m_ui_type = GncOptionUIType::INTERNAL;
    }
protected:
    OptionUIItem(GncOptionUIType ui_type) :
        m_ui_item{nullptr}, m_ui_type{ui_type} {}
    OptionUIItem(const OptionUIItem&) = default;
    OptionUIItem(OptionUIItem&&) = default;
    ~OptionUIItem() = default;
    OptionUIItem& operator=(const OptionUIItem&) = default;
    OptionUIItem& operator=(OptionUIItem&&) = default;
private:
    GncOptionUIItem* m_ui_item;
    GncOptionUIType m_ui_type;
};

/* These will work when m_value is a built-in class; GnuCash class and container
 * values will need specialization unless they happen to define operators << and
 * >>.
 * Note that SWIG 3.0.12 chokes on the typename = std::enable_if_t<> form so we
 * have to use the non-type parameter form.
 */
template<class OptionValueClass, typename std::enable_if_t<std::is_base_of_v<OptionClassifier, std::decay_t<OptionValueClass>>, int> = 0>
std::ostream& operator<<(std::ostream& oss, const OptionValueClass& opt)
{
    oss << opt.get_value();
    return oss;
}

template<class OptionValueClass, typename std::enable_if_t<std::is_base_of_v<OptionClassifier, std::decay_t<OptionValueClass>>, int> = 0>
std::istream& operator>>(std::istream& iss, OptionValueClass& opt)
{
    std::decay_t<decltype(opt.get_value())> value;
    iss >> value;
    opt.set_value(value);
    return iss;
}

template <typename ValueType>
class GncOptionValue :
    public OptionClassifier, public OptionUIItem
{
public:
    GncOptionValue<ValueType>(const char* section, const char* name,
                              const char* key, const char* doc_string,
                              ValueType value,
                              GncOptionUIType ui_type = GncOptionUIType::INTERNAL) :
        OptionClassifier{section, name, key, doc_string},
        OptionUIItem(ui_type),
        m_value{value}, m_default_value{value} {}
    GncOptionValue<ValueType>(const GncOptionValue<ValueType>&) = default;
    GncOptionValue<ValueType>(GncOptionValue<ValueType>&&) = default;
    GncOptionValue<ValueType>& operator=(const GncOptionValue<ValueType>&) = default;
    GncOptionValue<ValueType>& operator=(GncOptionValue<ValueType>&&) = default;
    ValueType get_value() const { return m_value; }
    ValueType get_default_value() const { return m_default_value; }
    void set_value(ValueType new_value) { m_value = new_value; }
    bool is_changed() const noexcept { return m_value != m_default_value; }
private:
    ValueType m_value;
    ValueType m_default_value;
};

QofInstance* qof_instance_from_string(const std::string& str,
                                      GncOptionUIType type);
std::string qof_instance_to_string(const QofInstance* inst);

template<> inline std::ostream&
operator<< <GncOptionValue<bool>>(std::ostream& oss,
                                  const GncOptionValue<bool>& opt)
{
    oss << (opt.get_value() ? "#t" : "#f");
    return oss;
}

template<> inline std::ostream&
operator<< <GncOptionValue<QofInstance*>>(std::ostream& oss,
                                       const GncOptionValue<QofInstance*>& opt)
{
    oss << qof_instance_to_string(opt.get_value());
    return oss;
}

template<> inline std::istream&
operator>> <GncOptionValue<bool>>(std::istream& iss,
                                  GncOptionValue<bool>& opt)
{
    std::string instr;
    iss >> instr;
    opt.set_value(instr == "#t" ? true : false);
    return iss;
}

template<> inline std::istream&
operator>> <GncOptionValue<QofInstance*>>(std::istream& iss,
                                       GncOptionValue<QofInstance*>& opt)
{
    std::string instr;
    iss >> instr;
    opt.set_value(qof_instance_from_string(instr, opt.get_ui_type()));
    return iss;
}

template <typename ValueType>
class GncOptionValidatedValue :
    public OptionClassifier, public OptionUIItem
{
public:
    GncOptionValidatedValue<ValueType>(const char* section, const char* name,
                                       const char* key, const char* doc_string,
                                       ValueType value,
                                       std::function<bool(ValueType)>validator,
                                       GncOptionUIType ui_type = GncOptionUIType::INTERNAL
        ) :
        OptionClassifier{section, name, key, doc_string},
        OptionUIItem(ui_type),
        m_value{value}, m_default_value{value}, m_validator{validator}
        {
            if (!this->validate(value))
            throw std::invalid_argument("Attempt to create GncValidatedOption with bad value.");
        }
    GncOptionValidatedValue<ValueType>(const char* section, const char* name,
                                       const char* key, const char* doc_string,
                                       ValueType value,
                                       std::function<bool(ValueType)>validator,
                                       ValueType val_data) :
        OptionClassifier{section, name, key, doc_string}, m_value{value},
        m_default_value{value}, m_validator{validator}, m_validation_data{val_data}
    {
            if (!this->validate(value))
            throw std::invalid_argument("Attempt to create GncValidatedOption with bad value.");
    }
    GncOptionValidatedValue<ValueType>(const GncOptionValidatedValue<ValueType>&) = default;
    GncOptionValidatedValue<ValueType>(GncOptionValidatedValue<ValueType>&&) = default;
    GncOptionValidatedValue<ValueType>& operator=(const GncOptionValidatedValue<ValueType>&) = default;
    GncOptionValidatedValue<ValueType>& operator=(GncOptionValidatedValue<ValueType>&&) = default;
    ValueType get_value() const { return m_value; }
    ValueType get_default_value() const { return m_default_value; }
    bool validate(ValueType value) const { return m_validator(value); }
    void set_value(ValueType value)
    {
        if (this->validate(value))
            m_value = value;
        else
            throw std::invalid_argument("Validation failed, value not set.");
    }
    bool is_changed() const noexcept { return m_value != m_default_value; }
private:
    ValueType m_value;
    ValueType m_default_value;
    std::function<bool(ValueType)> m_validator;                         //11
    ValueType m_validation_data;
};

template<> inline std::ostream&
operator<< <GncOptionValidatedValue<QofInstance*>>(std::ostream& oss,
                                       const GncOptionValidatedValue<QofInstance*>& opt)
{
        oss << qof_instance_to_string(opt.get_value());
        std::cerr << qof_instance_to_string(opt.get_value());
        return oss;
}

template<> inline std::istream&
operator>> <GncOptionValidatedValue<QofInstance*>>(std::istream& iss,
                                       GncOptionValidatedValue<QofInstance*>& opt)
{
    std::string instr;
    iss >> instr;
    opt.set_value(qof_instance_from_string(instr, opt.get_ui_type()));
    return iss;
}

/**
 * Used for numeric ranges and plot sizes.
 */

template <typename ValueType>
class GncOptionRangeValue :
    public OptionClassifier, public OptionUIItem
{
public:
    GncOptionRangeValue<ValueType>(const char* section, const char* name,
                                   const char* key, const char* doc_string,
                                   ValueType value, ValueType min,
                                   ValueType max, ValueType step) :
        OptionClassifier{section, name, key, doc_string},
        OptionUIItem(GncOptionUIType::NUMBER_RANGE),
        m_value{value >= min && value <= max ? value : min},
        m_default_value{value >= min && value <= max ? value : min},
        m_min{min}, m_max{max}, m_step{step} {}

    GncOptionRangeValue<ValueType>(const GncOptionRangeValue<ValueType>&) = default;
    GncOptionRangeValue<ValueType>(GncOptionRangeValue<ValueType>&&) = default;
    GncOptionRangeValue<ValueType>& operator=(const GncOptionRangeValue<ValueType>&) = default;
    GncOptionRangeValue<ValueType>& operator=(GncOptionRangeValue<ValueType>&&) = default;
    ValueType get_value() const { return m_value; }
    ValueType get_default_value() const { return m_default_value; }
    bool validate(ValueType value) { return value >= m_min && value <= m_max; }
    void set_value(ValueType value)
    {
        if (this->validate(value))
            m_value = value;
        else
            throw std::invalid_argument("Validation failed, value not set.");
    }
    bool is_changed() const noexcept { return m_value != m_default_value; }
private:
    ValueType m_value;
    ValueType m_default_value;
    ValueType m_min;
    ValueType m_max;
    ValueType m_step;
};

/** MultiChoice options have a vector of valid options
 * (GncMultiChoiceOptionChoices) and validate the selection as being one of
 * those values. The value is the index of the selected item in the vector. The
 * tuple contains three strings, a key, a display
 * name and a brief description for the tooltip. Both name and description
 * should be localized at the point of use. 
 *
 *
 */
using GncMultiChoiceOptionEntry = std::tuple<const std::string,
                                             const std::string,
                                             const std::string>;
using GncMultiChoiceOptionChoices = std::vector<GncMultiChoiceOptionEntry>;

class GncOptionMultichoiceValue :
    public OptionClassifier, public OptionUIItem
{
public:
    GncOptionMultichoiceValue(const char* section, const char* name,
                              const char* key, const char* doc_string,
                              const char* value,
                              GncMultiChoiceOptionChoices&& choices,
                              GncOptionUIType ui_type = GncOptionUIType::MULTICHOICE) :
        OptionClassifier{section, name, key, doc_string},
        OptionUIItem(ui_type),
        m_value{}, m_default_value{}, m_choices{std::move(choices)} {
            if (value)
            {
                if (auto index = find_key(value);
                    index != std::numeric_limits<std::size_t>::max())
                {
                    m_value = index;
                    m_default_value = index;
                }
            }
        }

    GncOptionMultichoiceValue(const GncOptionMultichoiceValue&) = default;
    GncOptionMultichoiceValue(GncOptionMultichoiceValue&&) = default;
    GncOptionMultichoiceValue& operator=(const GncOptionMultichoiceValue&) = default;
    GncOptionMultichoiceValue& operator=(GncOptionMultichoiceValue&&) = default;

    const std::string& get_value() const
    {
        return std::get<0>(m_choices.at(m_value));
    }
    const std::string& get_default_value() const
    {
        return std::get<0>(m_choices.at(m_default_value));
    }
     bool validate(const std::string& value) const noexcept
    {
        auto index = find_key(value);
        return index != std::numeric_limits<std::size_t>::max();

    }
    void set_value(const std::string& value)
    {
        auto index = find_key(value);
        if (index != std::numeric_limits<std::size_t>::max())
            m_value = index;
        else
            throw std::invalid_argument("Value not a valid choice.");

    }
    std::size_t num_permissible_values() const noexcept
    {
        return m_choices.size();
    }
    std::size_t permissible_value_index(const std::string& key) const noexcept
    {
            return find_key(key);
    }
    const std::string& permissible_value(std::size_t index) const
    {
        return std::get<0>(m_choices.at(index));
    }
    const std::string& permissible_value_name(std::size_t index) const
    {
        return std::get<1>(m_choices.at(index));
    }
    const std::string& permissible_value_description(std::size_t index) const
    {
        return std::get<2>(m_choices.at(index));
    }
    bool is_changed() const noexcept { return m_value != m_default_value; }
private:
    std::size_t find_key (const std::string& key) const noexcept
    {
        auto iter = std::find_if(m_choices.begin(), m_choices.end(),
                              [key](auto choice) {
                                  return std::get<0>(choice) == key; });
        if (iter != m_choices.end())
            return iter - m_choices.begin();
        else
            return std::numeric_limits<std::size_t>::max();

    }
    std::size_t m_value;
    std::size_t m_default_value;
    GncMultiChoiceOptionChoices m_choices;
};

/** Account options
 *
 * Set one or more accounts on which to report, optionally restricted to certain
 * account types. Many calls to make-account-list-option will pass a get-default
 * function that retrieves all of the accounts of a list of types.
 *
 * Some reports (examples/daily-reports.scm and standard/ account-piechart.scm,
 * advanced-portfolio.scm, category-barchart.scm, net-charts.scm, and
 * portfolio.scm) also provide a validator that rejects accounts that don't meet
 * an account-type criterion.
 *
 * There are two types of option, account-list which permits more than one
 * account selection and account-sel, which doesn't.
 *

 */

using GncOptionAccountList = std::vector<const Account*>;
using GncOptionAccountTypeList = std::vector<GNCAccountType>;

class GncOptionAccountValue :
    public OptionClassifier, public OptionUIItem
{
public:
    GncOptionAccountValue(const char* section, const char* name,
                          const char* key, const char* doc_string,
                          GncOptionUIType ui_type) :
        OptionClassifier{section, name, key, doc_string},
        OptionUIItem(ui_type), m_value{}, m_default_value{}, m_allowed{} {}

    GncOptionAccountValue(const char* section, const char* name,
                          const char* key, const char* doc_string,
                          GncOptionUIType ui_type,
                          const GncOptionAccountList& value) :
        OptionClassifier{section, name, key, doc_string},
        OptionUIItem(ui_type),
        m_value{value},
        m_default_value{std::move(value)}, m_allowed{} {}
    GncOptionAccountValue(const char* section, const char* name,
                          const char* key, const char* doc_string,
                          GncOptionUIType ui_type,
                          GncOptionAccountTypeList&& allowed) :
        OptionClassifier{section, name, key, doc_string},
        OptionUIItem(ui_type),
        m_value{},
        m_default_value{}, m_allowed{std::move(allowed)} {}
    GncOptionAccountValue(const char* section, const char* name,
                          const char* key, const char* doc_string,
                          GncOptionUIType ui_type,
                          const GncOptionAccountList& value,
                          GncOptionAccountTypeList&& allowed) :
        OptionClassifier{section, name, key, doc_string},
        OptionUIItem(ui_type),
        m_value{},
        m_default_value{}, m_allowed{std::move(allowed)} {
            if (!validate(value))
                throw std::invalid_argument("Supplied Value not in allowed set.");
            m_value = value;
            m_default_value = std::move(value);
        }

    const GncOptionAccountList& get_value() const { return m_value; }
    const GncOptionAccountList& get_default_value() const { return m_default_value; }
    bool validate (const GncOptionAccountList& values) const;
    void set_value (const GncOptionAccountList& values) {
        if (validate(values))
            //throw!
            m_value = values;
    }
    bool is_changed() const noexcept { return m_value != m_default_value; }
private:
    GncOptionAccountList m_value;
    GncOptionAccountList m_default_value;
    GncOptionAccountTypeList m_allowed;
};

template<> inline std::ostream&
operator<< <GncOptionAccountValue>(std::ostream& oss,
                                       const GncOptionAccountValue& opt)
{
    auto values{opt.get_value()};
    for (auto value : values)
        oss << qof_instance_to_string(QOF_INSTANCE(value)) << " ";
    return oss;
}

template<> inline std::istream&
operator>> <GncOptionAccountValue>(std::istream& iss,
                                   GncOptionAccountValue& opt)
{
    GncOptionAccountList values;
    while (true)
    {
        std::string str;
        std::getline(iss, str, ' ');
        if (!str.empty())
            values.emplace_back((Account*)qof_instance_from_string(str, opt.get_ui_type()));
        else
            break;
    }
    opt.set_value(values);
    iss.clear();
    return iss;
}
/** Date options
 * A legal date value is a pair of either and a RelativeDatePeriod, the absolute
 * flag and a time64, or for legacy purposes the absolute flag and a timespec.
 *
 * The original design allowed custom RelativeDatePeriods, but that facility is
 * unused so we'll go with compiled-in enums.
 */
/*
gnc-date-option-show-time? -- option_data[1]
gnc-date-option-get-subtype -- option_data[0]
gnc-date-option-value-type m_value
gnc-date-option-absolute-time m_type == DateTyupe::Absolute
gnc-date-option-relative-time m_type != DateTyupe::Absolute
 */

enum class RelativeDatePeriod : int
{
    ABSOLUTE = -1,
    TODAY,
    START_THIS_MONTH,
    END_THIS_MONTH,
    START_PREV_MONTH,
    END_PREV_MONTH,
    START_CURRENT_QUARTER,
    END_CURRENT_QUARTER,
    START_PREV_QUARTER,
    END_PREV_QUARTER,
    START_CAL_YEAR,
    END_CAL_YEAR,
    START_PREV_YEAR,
    END_PREV_YEAR,
    START_ACCOUNTING_PERIOD,
    END_ACCOUNTING_PERIOD
};

class GncOptionDateValue : public OptionClassifier, public OptionUIItem
{
public:
    GncOptionDateValue(const char* section, const char* name,
                              const char* key, const char* doc_string) :
        OptionClassifier{section, name, key, doc_string},
        OptionUIItem(GncOptionUIType::DATE),
        m_period{RelativeDatePeriod::END_ACCOUNTING_PERIOD},
        m_date{INT64_MAX},
        m_default_period{RelativeDatePeriod::END_ACCOUNTING_PERIOD},
        m_default_date{INT64_MAX} {}
    GncOptionDateValue(const char* section, const char* name,
                       const char* key, const char* doc_string,
                       time64 time) :
        OptionClassifier{section, name, key, doc_string},
        OptionUIItem(GncOptionUIType::DATE),
        m_period{RelativeDatePeriod::ABSOLUTE}, m_date{time},
        m_default_period{RelativeDatePeriod::ABSOLUTE}, m_default_date{time} {}
    GncOptionDateValue(const char* section, const char* name,
                       const char* key, const char* doc_string,
                       const RelativeDatePeriod period) :
        OptionClassifier{section, name, key, doc_string},
        OptionUIItem(GncOptionUIType::DATE),
        m_period{period}, m_date{INT64_MAX},
        m_default_period{period}, m_default_date{INT64_MAX} {}
        GncOptionDateValue(const GncOptionDateValue&) = default;
        GncOptionDateValue(GncOptionDateValue&&) = default;
        GncOptionDateValue& operator=(const GncOptionDateValue&) = default;
        GncOptionDateValue& operator=(GncOptionDateValue&&) = default;
    time64 get_value() const;
    time64 get_default_value() const { return static_cast<time64>(GncDateTime()); }
    std::ostream& out_stream(std::ostream& oss) const noexcept;
    std::istream& in_stream(std::istream& iss);
    void set_value(RelativeDatePeriod value) {
        m_period = value;
        m_date = INT64_MAX;
    }
    void set_value(time64 time) {
        m_period = RelativeDatePeriod::ABSOLUTE;
        m_date = time;
    }
    bool is_changed() const noexcept { return m_period != m_default_period &&
            m_date != m_default_date; }
private:
    RelativeDatePeriod m_period;
    time64 m_date;
    RelativeDatePeriod m_default_period;
    time64 m_default_date;
};

template<> inline std::ostream&
operator<< <GncOptionDateValue>(std::ostream& oss,
                                       const GncOptionDateValue& opt)
{
    return opt.out_stream(oss);
}

template<> inline std::istream&
operator>> <GncOptionDateValue>(std::istream& iss,
                                   GncOptionDateValue& opt)
{
    return opt.in_stream(iss);
}

using GncOptionVariant = std::variant<GncOptionValue<std::string>,
                                      GncOptionValue<bool>,
                                      GncOptionValue<int64_t>,
                                      GncOptionValue<QofInstance*>,
                                      GncOptionAccountValue,
                                      GncOptionMultichoiceValue,
                                      GncOptionRangeValue<int>,
                                      GncOptionRangeValue<double>,
                                      GncOptionValidatedValue<QofInstance*>,
                                      GncOptionDateValue>;

class GncOption
{
public:
    template <typename OptionType>
    GncOption(OptionType option) : m_option{option} {}

    template <typename ValueType>
    GncOption(const char* section, const char* name,
              const char* key, const char* doc_string,
              ValueType value,
              GncOptionUIType ui_type = GncOptionUIType::INTERNAL) :
        m_option{GncOptionValue<ValueType> {
            section, name, key, doc_string, value, ui_type}} {}

    template <typename ValueType> ValueType get_value() const
    {
        return std::visit([](const auto& option)->ValueType {
                if constexpr (std::is_same_v<std::decay_t<decltype(option.get_value())>, std::decay_t<ValueType>>)
                    return option.get_value();
                return ValueType {};
            }, m_option);
    }

    template <typename ValueType> ValueType get_default_value() const
    {
        return std::visit([](const auto& option)->ValueType {
                if constexpr (std::is_same_v<std::decay_t<decltype(option.get_value())>, std::decay_t<ValueType>>)
                    return option.get_default_value();
                return ValueType {};
            }, m_option);

    }

    template <typename ValueType> void set_value(ValueType value)
    {
        std::visit([value](auto& option) {
                if constexpr
                    (std::is_same_v<std::decay_t<decltype(option.get_value())>,
                      std::decay_t<ValueType>> ||
                     (std::is_same_v<std::decay_t<decltype(option)>,
                      GncOptionDateValue> &&
                      std::is_same_v<std::decay_t<ValueType>,
                      RelativeDatePeriod>))
                                 option.set_value(value);
            }, m_option);
    }
    const std::string& get_section() const
    {
        return std::visit([](const auto& option)->const std::string& {
                return option.m_section;
            }, m_option);
    }
    const std::string& get_name() const
    {
        return std::visit([](const auto& option)->const std::string& {
                return option.m_name;
            }, m_option);
    }
    const std::string& get_key() const
    {
        return std::visit([](const auto& option)->const std::string& {
                return option.m_sort_tag;
            }, m_option);
    }
    const std::string& get_docstring() const
    {
          return std::visit([](const auto& option)->const std::string& {
                return option.m_doc_string;
              }, m_option);
    }
    void set_ui_item(GncOptionUIItem* ui_elem)
    {
        std::visit([ui_elem](auto& option) {
                option.set_ui_item(ui_elem);
            }, m_option);
    }
    const GncOptionUIType get_ui_type() const
    {
        return std::visit([](const auto& option)->GncOptionUIType {
                return option.get_ui_type();
            }, m_option);
    }
    GncOptionUIItem* const get_ui_item() const
    {
        return std::visit([](const auto& option)->GncOptionUIItem* {
                return option.get_ui_item();
            }, m_option);
    }
    void make_internal()
    {
        std::visit([](auto& option) {
                option.make_internal();
            }, m_option);
    }
    bool is_changed()
    {
        return std::visit([](const auto& option)->bool {
                return option.is_changed();
            }, m_option);
    }

    template<typename ValueType>
    bool validate(ValueType value) const {
        return std::visit([value] (const auto& option) -> bool {
                              if constexpr ((std::is_same_v<std::decay_t<decltype(option)>,
                                                   GncOptionMultichoiceValue> &&
                                      std::is_same_v<std::decay_t<ValueType>,
                                                     std::string>) ||
                                            std::is_same_v<std::decay_t<decltype(option)>,
                                            GncOptionValidatedValue<ValueType>>)
                                        return option.validate(value);
                       else
                           return false;
                   }, m_option);
    }

    std::size_t num_permissible_values() const {
        return std::visit([] (const auto& option) -> size_t {
                              if constexpr (std::is_same_v<std::decay_t<decltype(option)>,
                                                    GncOptionMultichoiceValue>)
                                        return option.num_permissible_values();
                       else
                           return std::numeric_limits<std::size_t>::max();
                   }, m_option);
    }

    std::size_t permissible_value_index(const std::string& value) const {
        return std::visit([&value] (const auto& option) -> size_t {
                              std::cerr << typeid(option).name() << std::endl;
                              if constexpr (std::is_same_v<std::decay_t<decltype(option)>,
                                                    GncOptionMultichoiceValue>)
                                        return option.permissible_value_index(value);
                       else
                           return std::numeric_limits<std::size_t>::max();;
                   }, m_option);
    }

    const std::string& permissible_value(std::size_t index) const {
        return std::visit([index] (const auto& option) -> const std::string& {
                              if constexpr (std::is_same_v<std::decay_t<decltype(option)>,
                                     GncOptionMultichoiceValue>)
                                        return option.permissible_value(index);
                       else
                           return c_empty_string;
                   }, m_option);
    }

    const std::string& permissible_value_name(std::size_t index) const {
        return std::visit([index] (const auto& option) -> const std::string& {
                              if constexpr (std::is_same_v<std::decay_t<decltype(option)>,
                                     GncOptionMultichoiceValue>)
                                        return option.permissible_value_name(index);
                       else
                           return c_empty_string;
                   }, m_option);
    }

    const std::string& permissible_value_description(std::size_t index) const {
        return std::visit([index] (const auto& option) -> const std::string& {
                              if constexpr (std::is_same_v<std::decay_t<decltype(option)>,
                                     GncOptionMultichoiceValue>)
                                        return option.permissible_value_description(index);
                       else
                           return c_empty_string;
                   }, m_option);
    }

    std::ostream& out_stream(std::ostream& oss) const
    {
            return std::visit([&oss](auto& option) -> std::ostream& {
            oss << option;
            return oss;
        }, m_option);
    }
    std::istream& in_stream(std::istream& iss)
    {
    return std::visit([&iss](auto& option) -> std::istream& {
            iss >> option;
            return iss;
        }, m_option);
    }

    GncOptionVariant& _get_option() const { return m_option; }
private:
    mutable GncOptionVariant m_option;
};

inline std::ostream&
operator<<(std::ostream& oss, const GncOption& opt)
{
    return opt.out_stream(oss);
}

inline std::istream&
operator>>(std::istream& iss, GncOption& opt)
{
    return opt.in_stream(iss);
}

#endif //GNC_OPTION_HPP_