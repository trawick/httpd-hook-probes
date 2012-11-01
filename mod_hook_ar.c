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

#include "apr_strings.h"

#include "httpd.h"
#include "http_log.h"
#include "ap_hooks.h"

#include "mod_crashstate.h"

APLOG_USE_MODULE(hook_ar);

static APR_OPTIONAL_FN_TYPE(crashstate_save_state) *save_state;

void *ap_hook_probe_entry(const char *name)
{
    switch(name[0]) {
    case 'a':
        if (!strcmp(name, "access_checker")
            || !strcmp(name, "access_checker_ex")
            || !strcmp(name, "auth_checker")) {
            return (void *)1;
        }
        break;
    case 'c':
        if (!strcmp(name, "check_user_id")) {
            return (void *)1;
        }
        break;
    case 'f':
        if (!strcmp(name, "fixups")) {
            return (void *)1;
        }
        break;
    case 'h':
        if (!strcmp(name, "handler")) {
            return (void *)1;
        }
        break;
    case 'm':
        if (!strcmp(name, "map_to_storage")) {
            return (void *)1;
        }
        break;
    case 'p':
        if (!strcmp(name, "post_read_request")) {
            return (void *)1;
        }
        break;
    case 'q':
        if (!strcmp(name, "quick_handler")) {
            return (void *)1;
        }
        break;
    case 't':
        if (!strcmp(name, "type_checker")
            || !strcmp(name, "translate_name")) {
            return (void *)1;
        }
        break;
    }
    return NULL;
}

void ap_hook_probe_invoke(void *ud, const char *name, const char *src, const void *arg1)
{
    if (ud) {
        request_rec *r = (request_rec *)arg1;
        const char *am = apr_psprintf(r->pool, "%s/%s", src, name);

        apr_table_set(r->notes, "ActiveModule", am);
        if (save_state) {
            save_state(am);
        }
    }
}

void ap_hook_probe_complete(void *ud, const char *name, const char *src, void *rv, const void *arg1)
{
    if (ud) {
        int rc = (int)rv; /* works with the hooks for which this is called */
        request_rec *r = (request_rec *)arg1;

        apr_table_unset(r->notes, "ActiveModule");
        if (save_state) {
            save_state(NULL);
        }
        if (rc != HTTP_OK && rc != DECLINED && rc != OK) {
            ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, r,
                          "%s %s -> %d",
                          src, name, rc);
            apr_table_set(r->notes, "RequestFailer",
                          apr_psprintf(r->pool, "%s/%d/%s", src, rc, name));
        }
    }
}

const void *ap_hook_probe_arg1(const void *arg1, ...)
{
    return arg1;
}

static void hook_ar_optional_fn_retrieve(void)
{
    save_state = APR_RETRIEVE_OPTIONAL_FN(crashstate_save_state);
}

static void hook_ar_register_hooks(apr_pool_t *p)
{
    ap_hook_optional_fn_retrieve(hook_ar_optional_fn_retrieve, NULL, NULL,
                                 APR_HOOK_MIDDLE);
}

module AP_MODULE_DECLARE_DATA hook_ar_module =
{
    STANDARD20_MODULE_STUFF,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    hook_ar_register_hooks,
};
