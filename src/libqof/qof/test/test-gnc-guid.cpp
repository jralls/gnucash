/********************************************************************
 * utest-gnc-date.c: GLib g_test test suite for gnc-date.c.	    *
 * Copyright 2012 John Ralls <jralls@ceridwen.us>		    *
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
 * along with this program; if not, you can retrieve it from        *
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.html            *
 * or contact:                                                      *
 *                                                                  *
 * Free Software Foundation           Voice:  +1-617-542-5942       *
 * 51 Franklin Street, Fifth Floor    Fax:    +1-617-542-2652       *
 * Boston, MA  02110-1301,  USA       gnu@gnu.org                   *
 ********************************************************************/
extern "C"
{
#include <config.h>
#include <string.h>
#include <glib.h>
#include <unittest-support.h>
void test_suite_gnc_guid ( void );
}

/*Can be included as c++, because it's c++ tolerant*/
#include "../guid.h"
#include <string>
#include <iostream>

using namespace std;

static const gchar *suitename = "/qof/gnc-guid";

/*Create a GUID and free it.*/
static void test_gnc_create_guid ( void ){
    GncGUID * guid { guid_malloc()};
    g_assert ( guid != nullptr);
    guid_new(guid);
    guid_free(guid);
}

/*We create a GUID, create a copy, and compare them to ensure they're not different.*/
static void test_gnc_guid_copy ( void ) {
    GncGUID * const guid { guid_malloc()} ;
    g_assert ( guid != nullptr);
    guid_new(guid);
    GncGUID * const cp { guid_copy ( guid ) };

    for ( unsigned i { 0} ; i < GUID_DATA_SIZE; ++i)
        g_assert ( guid->data[i] == cp->data[i] );

    guid_free(cp);
    guid_free(guid);
}

/* We create a GUID, then convert it to a string using the two methods
defined in the guid api. We then compare them.*/
static void test_gnc_guid_to_string ( void ) {
    GncGUID * const guid { guid_malloc()};
    g_assert ( guid != nullptr );
    guid_new(guid);
    string message {" using guid_to_string (depracated): "};
    /*don't free the return value of guid_to_string!*/
    string const guidstr { guid_to_string ( guid ) };
    message += guidstr;
    g_test_message(message.c_str());
    message = " using guid_to_string_buff: ";
    gchar guidstrp2[GUID_ENCODING_LENGTH+1];
    gchar * const ret { guid_to_string_buff(guid, guidstrp2) };
    g_assert ( ret == guidstrp2 + GUID_ENCODING_LENGTH );
    string const guidstr2 { guidstrp2 };
    message += guidstr2;
    g_test_message(message.c_str());

    g_assert ( guidstr2 == guidstr );
    guid_free(guid);
}

void test_suite_gnc_guid (void)
{
    guid_init();
    GNC_TEST_ADD_FUNC (suitename, "gnc create guid", test_gnc_create_guid);
    GNC_TEST_ADD_FUNC (suitename, "gnc copy guid", test_gnc_guid_copy);
    GNC_TEST_ADD_FUNC (suitename, "gnc guid to string", test_gnc_guid_to_string);
    guid_shutdown();
}

