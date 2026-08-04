/* /////////////////////////////////////////////////////////////////////////
 * File:    test.unit.file_crc.c
 *
 * Purpose: Unit-tests for WilliamsCRC file / file-handle CRC calculation.
 *
 * Created: 5th August 2026
 * Updated: 5th August 2026
 *
 * ////////////////////////////////////////////////////////////////////// */


/* /////////////////////////////////////////////////////////////////////////
 * includes
 */

#include <WilliamsCRC/WilliamsCRC.h>

#include <xtests/terse-api.h>

#include <stlsoft/stlsoft.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* /////////////////////////////////////////////////////////////////////////
 * forward declarations
 */

static void test_file_matches_block(void);
static void test_file_max_zero_reads_all(void);
static void test_file_max_partial(void);
static void test_file_handle_matches_path(void);
static void test_file_handle_max_partial(void);
static void test_file_missing_path(void);
static void test_file_pcb_read_null_ok(void);


/* /////////////////////////////////////////////////////////////////////////
 * helpers
 */

static int
write_temp_file_(
    char const*     path
,   void const*     pv
,   size_t          cb
)
{
    FILE* f = fopen(path, "wb");

    if (NULL == f)
    {
        return (0 != errno) ? errno : EMFILE;
    }

    if (cb != fwrite(pv, 1, cb, f))
    {
        int const e = (0 != errno) ? errno : EIO;

        fclose(f);
        remove(path);

        return e;
    }

    if (0 != fclose(f))
    {
        int const e = (0 != errno) ? errno : EIO;

        remove(path);

        return e;
    }

    return 0;
}


/* /////////////////////////////////////////////////////////////////////////
 * main
 */

int main(int argc, char** argv)
{
    int retCode = EXIT_SUCCESS;
    int verbosity = 2;

    XTESTS_COMMANDLINE_PARSEVERBOSITY(argc, argv, &verbosity);

    if (XTESTS_START_RUNNER("test.unit.file_crc", verbosity))
    {
        XTESTS_RUN_CASE(test_file_matches_block);
        XTESTS_RUN_CASE(test_file_max_zero_reads_all);
        XTESTS_RUN_CASE(test_file_max_partial);
        XTESTS_RUN_CASE(test_file_handle_matches_path);
        XTESTS_RUN_CASE(test_file_handle_max_partial);
        XTESTS_RUN_CASE(test_file_missing_path);
        XTESTS_RUN_CASE(test_file_pcb_read_null_ok);

        XTESTS_PRINT_RESULTS();

        XTESTS_END_RUNNER_UPDATE_EXITCODE(&retCode);
    }

    return retCode;
}


/* /////////////////////////////////////////////////////////////////////////
 * test function implementations
 */

static void test_file_matches_block(void)
{
    char const* const path = "test.unit.file_crc.matches_block.tmp";
    char const* const s = "123456789";
    crc_result_t block_result = 0;
    crc_result_t file_result = 0;

    TEST_INT_EQ(0, WilliamsCRC_Init());
    TEST_INT_EQ(0, write_temp_file_(path, s, 9));

    TEST_INT_EQ(0, WilliamsCRC_CalculateBlockCrc(s, 9, 32, 0, &block_result));
    TEST_INT_EQ(0, WilliamsCRC_CalculateFileCrc(path, 32, 0, &file_result));
    TEST_INT_EQ(block_result, file_result);
    TEST_INT_EQ(0x1847dd70u, file_result);

    remove(path);
    WilliamsCRC_Uninit();
}

static void test_file_max_zero_reads_all(void)
{
    char const* const path = "test.unit.file_crc.max_zero.tmp";
    char const* const s = "Hello, WilliamsCRC!";
    crc_result_t result = 0;
    size_t num_read = 0;

    TEST_INT_EQ(0, WilliamsCRC_Init());
    TEST_INT_EQ(0, write_temp_file_(path, s, strlen(s)));

    TEST_INT_EQ(0, WilliamsCRC_CalculateFileCrcMax(path, 0, 32, 0, &result, &num_read));
    TEST_INT_EQ(strlen(s), num_read);
    TEST_INT_EQ(0x134bdaeau, result);

    remove(path);
    WilliamsCRC_Uninit();
}

static void test_file_max_partial(void)
{
    char const* const path = "test.unit.file_crc.max_partial.tmp";
    char const* const s = "Hello, WilliamsCRC!";
    crc_result_t file_result = 0;
    crc_result_t block_result = 0;
    size_t num_read = 0;
    size_t const limit = 5;

    TEST_INT_EQ(0, WilliamsCRC_Init());
    TEST_INT_EQ(0, write_temp_file_(path, s, strlen(s)));

    TEST_INT_EQ(0, WilliamsCRC_CalculateFileCrcMax(path, limit, 32, 0, &file_result, &num_read));
    TEST_INT_EQ(limit, num_read);

    TEST_INT_EQ(0, WilliamsCRC_CalculateBlockCrc(s, limit, 32, 0, &block_result));
    TEST_INT_EQ(block_result, file_result);

    remove(path);
    WilliamsCRC_Uninit();
}

static void test_file_handle_matches_path(void)
{
    char const* const path = "test.unit.file_crc.handle_matches.tmp";
    char const* const s = "123456789";
    crc_result_t path_result = 0;
    crc_result_t handle_result = 0;
    FILE* f;

    TEST_INT_EQ(0, WilliamsCRC_Init());
    TEST_INT_EQ(0, write_temp_file_(path, s, 9));

    TEST_INT_EQ(0, WilliamsCRC_CalculateFileCrc(path, 16, 0, &path_result));

    f = fopen(path, "rb");
    TEST_PTR_NE(NULL, f);

    TEST_INT_EQ(0, WilliamsCRC_CalculateFileHandleCrc(f, 16, 0, &handle_result));
    TEST_INT_EQ(path_result, handle_result);
    TEST_INT_EQ(0xd1d4u, handle_result);

    fclose(f);
    remove(path);
    WilliamsCRC_Uninit();
}

static void test_file_handle_max_partial(void)
{
    char const* const path = "test.unit.file_crc.handle_max.tmp";
    char const* const s = "Hello, WilliamsCRC!";
    crc_result_t handle_result = 0;
    crc_result_t block_result = 0;
    size_t num_read = 0;
    size_t const limit = 7;
    FILE* f;

    TEST_INT_EQ(0, WilliamsCRC_Init());
    TEST_INT_EQ(0, write_temp_file_(path, s, strlen(s)));

    f = fopen(path, "rb");
    TEST_PTR_NE(NULL, f);

    TEST_INT_EQ(0, WilliamsCRC_CalculateFileHandleCrcMax(f, limit, 32, 0, &handle_result, &num_read));
    TEST_INT_EQ(limit, num_read);

    TEST_INT_EQ(0, WilliamsCRC_CalculateBlockCrc(s, limit, 32, 0, &block_result));
    TEST_INT_EQ(block_result, handle_result);

    fclose(f);
    remove(path);
    WilliamsCRC_Uninit();
}

static void test_file_missing_path(void)
{
    crc_result_t result = 0;
    int rc;

    TEST_INT_EQ(0, WilliamsCRC_Init());

    rc = WilliamsCRC_CalculateFileCrc("test.unit.file_crc.does_not_exist.tmp", 32, 0, &result);

    TEST_INT_NE(0, rc);

    WilliamsCRC_Uninit();
}

static void test_file_pcb_read_null_ok(void)
{
    char const* const path = "test.unit.file_crc.pcb_null.tmp";
    char const* const s = "123456789";
    crc_result_t result = 0;

    TEST_INT_EQ(0, WilliamsCRC_Init());
    TEST_INT_EQ(0, write_temp_file_(path, s, 9));

    TEST_INT_EQ(0, WilliamsCRC_CalculateFileCrcMax(path, 0, 32, 0, &result, NULL));
    TEST_INT_EQ(0x1847dd70u, result);

    remove(path);
    WilliamsCRC_Uninit();
}


/* ///////////////////////////// end of file //////////////////////////// */
