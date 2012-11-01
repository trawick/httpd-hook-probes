/* Copyright 2012 Jeff Trawick, http://emptyhammock.com/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "httpd.h"
#include "http_config.h"
#include "http_log.h"
#include "http_main.h"
#include "ap_mpm.h"
#include "ap_mmn.h"

#include "mod_crashstate.h"

#if AP_MODULE_MAGIC_AT_LEAST(20120211, 0)
APLOG_USE_MODULE(crashstate);
#endif

static __thread const char *thread_crashstate;

static int crashstate_fatal_exception(ap_exception_info_t *ei)
{
    ap_log_error(APLOG_MARK, APLOG_NOTICE, 0, ap_server_conf,
                 "Crash state: %s",
                 thread_crashstate);
    return OK;
}

static void crashstate_save_state(const char *st)
{
    thread_crashstate = st;
}

static void crashstate_register_hooks(apr_pool_t *p)
{
    ap_hook_fatal_exception(crashstate_fatal_exception, NULL, NULL,
                            APR_HOOK_MIDDLE);
    APR_REGISTER_OPTIONAL_FN(crashstate_save_state);
}

module AP_MODULE_DECLARE_DATA crashstate_module = {
    STANDARD20_MODULE_STUFF,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    crashstate_register_hooks
};
