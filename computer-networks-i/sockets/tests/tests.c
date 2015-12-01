#include <string.h>

#include "protocol.h"
#include "vector.h"
#include "tests.h"

TEST(parse_message_simple, {
    client_message_t result;
    const char* message = "HOLA abc";

    ASSERT(ERROR_NONE == parse_client_message(message, &result));
    ASSERT(MESSAGE_TYPE_HELLO == result.type);
    ASSERT(strcmp(result.content.uid, "abc") == 0);
})

TEST(parse_message_list_events, {
    client_message_t result;
    const char* message = "LISTAR EVENTOS";

    ASSERT(ERROR_NONE == parse_client_message(message, &result));
    ASSERT(MESSAGE_TYPE_EVENT_LIST == result.type);
})

TEST(parse_message_list_request, {
    client_message_t result;
    const char* message = "LISTAR abc ecdf";

    ASSERT(ERROR_NONE == parse_client_message(message, &result));
    ASSERT(MESSAGE_TYPE_ASSISTANCE_LIST == result.type);

    ASSERT(strcmp(result.content.assistance_list_info.uid, "abc") == 0);
    ASSERT(strcmp(result.content.assistance_list_info.event_id, "ecdf") == 0);

})

TEST(parse_message_assistance, {
    client_message_t result;
    const char* message = "FICHAR abc def 20-11-2015";

    ASSERT(ERROR_NONE == parse_client_message(message, &result));
    ASSERT(MESSAGE_TYPE_ASSISTANCE == result.type);

    ASSERT(strcmp(result.content.assistance_info.event_id, "abc") == 0);
    ASSERT(strcmp(result.content.assistance_info.uid, "def") == 0);

    ASSERT(result.content.assistance_info.datetime.tm_year == 2015 - 1900);
    ASSERT(result.content.assistance_info.datetime.tm_mon == 10);

    char buff[20];
    strftime(buff, 20, "%d-%m-%Y", &result.content.assistance_info.datetime);
    ASSERT(strcmp(buff, "20-11-2015") == 0);
})

TEST(parse_failure_invalid_type, {
    client_message_t result;
    const char* message = "ABC abc";

    ASSERT(ERROR_INVALID_MESSAGE_TYPE == parse_client_message(message, &result));
})

TEST(parse_failure_extra_content, {
    client_message_t result;
    const char* message = "HOLA abc ";

    ASSERT(ERROR_UNEXPECTED_CONTENT == parse_client_message(message, &result));
})

TEST(parse_failure_too_long_ident, {
    client_message_t result;
    const char* message = "HOLA aaaaaaaaaaaaaaaaaaaaa";

    ASSERT(ERROR_EXPECTED_VALID_UID == parse_client_message(message, &result));
})

TEST(parse_failure_missing_ident, {
    client_message_t result;
    const char* message = "LISTAR abc ";

    ASSERT(ERROR_EXPECTED_VALID_EVENT_ID == parse_client_message(message, &result));
})

TEST(parse_failure_invalid_date, {
    client_message_t result;
    const char* message = "FICHAR abc def fweafew";

    ASSERT(ERROR_EXPECTED_VALID_DATE == parse_client_message(message, &result));
})

TEST(parse_failure_extra_newline, {
    client_message_t result;
    const char* message = "HOLA abc\n";

    ASSERT(ERROR_UNEXPECTED_CONTENT == parse_client_message(message, &result));
})

TEST(parse_failure_extra_newline_in_content, {
    client_message_t result;
    const char* message = "HOLA\nabc\n";

    ASSERT(ERROR_INVALID_MESSAGE_TYPE == parse_client_message(message, &result));
})

TEST(vector_base, {
    vector_t v;

    vector_init(&v, sizeof(int), 10);

    ASSERT(v.buffer != NULL);
    ASSERT(vector_size(&v) == 0);
    ASSERT(vector_capacity(&v) == 10);

    for (int i = 0; i < 20; ++i) {
        ASSERT(vector_push(&v, &i));
    }

    ASSERT(vector_size(&v) == 20);

    int test = 0;
    ASSERT(vector_get(&v, 19, &test));
    ASSERT(test == 19);

    ASSERT(vector_delete(&v, 18));
    ASSERT(vector_get(&v, 18, &test));
    ASSERT(test == 19);

    ASSERT(vector_size(&v) == 19);

    ASSERT(vector_delete(&v, 0));
    ASSERT(vector_get(&v, 0, &test));
    ASSERT(test == 1);

    ASSERT(vector_size(&v) == 18);

    vector_destroy(&v);
    ASSERT(v.buffer == NULL);
})

TEST(parse_event_simple, {
    protocol_event_t result;
    const char* str = "sabbdf#fdafwafew#2015-01-01#2015-02-01";

    ASSERT(ERROR_NONE == parse_event(str, &result));
    ASSERT(strcmp("sabbdf", result.id) == 0);
    ASSERT(strcmp(result.description, "fdafwafew") == 0);
})

TEST_MAIN({
    RUN_TEST(parse_message_simple);
    RUN_TEST(parse_message_list_events);
    RUN_TEST(parse_message_list_request);
    RUN_TEST(parse_message_assistance);
    RUN_TEST(parse_failure_invalid_type);
    RUN_TEST(parse_failure_extra_content);
    RUN_TEST(parse_failure_too_long_ident);
    RUN_TEST(parse_failure_missing_ident);
    RUN_TEST(parse_failure_invalid_date);
    RUN_TEST(parse_failure_extra_newline);
    RUN_TEST(parse_failure_extra_newline_in_content);

    RUN_TEST(vector_base);

    RUN_TEST(parse_event_simple);
})
