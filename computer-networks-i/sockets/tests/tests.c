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
    const char* message = "FICHAR abc def";

    ASSERT(ERROR_NONE == parse_client_message(message, &result));
    ASSERT(MESSAGE_TYPE_ASSISTANCE == result.type);

    ASSERT(strcmp(result.content.assistance_info.event_id, "abc") == 0);
    ASSERT(strcmp(result.content.assistance_info.uid, "def") == 0);
    time_t now = time(NULL);
    struct tm* tm = localtime(&now);
    ASSERT(result.content.assistance_info.datetime.tm_year == tm->tm_year);
    ASSERT(result.content.assistance_info.datetime.tm_mon == tm->tm_mon);
    ASSERT(result.content.assistance_info.datetime.tm_mday == tm->tm_mday);
    ASSERT(result.content.assistance_info.datetime.tm_hour == tm->tm_hour);
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

    ASSERT(vector_init(&v, sizeof(int), 10));

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

TEST(parse_event_simple_nonzero, {
    protocol_event_t result;
    const char* str = "sabbdf#fdafwafew#2015-01-01#2015-02-01";

    memset(&result, 1, sizeof(result));

    ASSERT(ERROR_NONE == parse_event(str, &result));
    ASSERT(strcmp("sabbdf", result.id) == 0);
    ASSERT(strcmp(result.description, "fdafwafew") == 0);
})

TEST(parse_event_complex, {
    protocol_event_t result;
    const char* str = "id_evento_1#descripción del evento 1#01/11/2015 09:00:00#01/12/2015 14:00:00";

    ASSERT(ERROR_NONE == parse_event(str, &result));
    ASSERT(strcmp(result.id, "id_evento_1") == 0);
    ASSERT(strcmp(result.description, "descripción del evento 1") == 0);
    ASSERT(strcmp(result.description, "descripción del evento 1") == 0);

    char buff[200];

    strftime(buff, sizeof(buff), "%d/%m/%Y %H:%M:%S", &result.starts_at);
    ASSERT(strcmp(buff, "01/11/2015 09:00:00") == 0);

    strftime(buff, sizeof(buff), "%d/%m/%Y %H:%M:%S", &result.ends_at);
    ASSERT(strcmp(buff, "01/12/2015 14:00:00") == 0);
})


TEST(parse_failure_event_invalid_date, {
    protocol_event_t result;
    const char* str = "sabbdf#fdafwafew#fsfs#2015-02-01";

    ASSERT(ERROR_EXPECTED_VALID_DATE == parse_event(str, &result));
})

TEST(parse_failure_event_missing_separator, {
    protocol_event_t result;
    const char* str = "sabbdf#fdafwafew#2015-02-01";

    ASSERT(ERROR_EXPECTED_SEPARATOR == parse_event(str, &result));
})

TEST(parse_assistance_simple, {
    protocol_assistance_t result;
    const char* str = "sabbdf#fdafwafew#2015-02-01";

    ASSERT(ERROR_NONE == parse_assistance(str, &result));
    ASSERT(strcmp(result.uid, "sabbdf") == 0);
    ASSERT(strcmp(result.event_id, "fdafwafew") == 0);
})

TEST(parse_invitation_simple, {
    protocol_invitation_t result;
    const char* str = "sabbdf#fdafwafew";

    ASSERT(ERROR_NONE == parse_invitation(str, &result));
    ASSERT(strcmp(result.uid, "sabbdf") == 0);
    ASSERT(strcmp(result.event_id, "fdafwafew") == 0);
})

TEST(parse_user_simple, {
    protocol_user_t result;
    const char* str = "sabbdf";

    ASSERT(ERROR_NONE == parse_user(str, &result));
    ASSERT(strcmp(result.id, "sabbdf") == 0);

    const char* str_2 = "sabbdf#fdafew";
    ASSERT(ERROR_UNEXPECTED_CONTENT == parse_user(str_2, &result));
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
    RUN_TEST(parse_failure_extra_newline);
    RUN_TEST(parse_failure_extra_newline_in_content);

    RUN_TEST(vector_base);

    RUN_TEST(parse_event_simple);
    RUN_TEST(parse_event_simple_nonzero);
    RUN_TEST(parse_event_complex);
    RUN_TEST(parse_failure_event_invalid_date);
    RUN_TEST(parse_failure_event_missing_separator);

    RUN_TEST(parse_assistance_simple);

    RUN_TEST(parse_invitation_simple);

    RUN_TEST(parse_user_simple);
})
