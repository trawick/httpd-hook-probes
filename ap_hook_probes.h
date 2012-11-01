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

#ifndef AP_HOOK_PROBES_H
#define AP_HOOK_PROBES_H

#undef APR_HOOK_INT_DCL_UD
#undef APR_HOOK_PROBE_ENTRY
#undef APR_HOOK_PROBE_RETURN
#undef APR_HOOK_PROBE_INVOKE
#undef APR_HOOK_PROBE_COMPLETE

#define APR_HOOK_INT_DCL_UD void *ud = NULL

#define APR_HOOK_PROBE_ENTRY(ud,ns,name,args) \
    ud = ap_hook_probe_entry(#name)

#define APR_HOOK_PROBE_INVOKE(ud,ns,name,src,args) \
    ap_hook_probe_invoke(ud, #name, src, ap_hook_probe_arg1 args)

#define APR_HOOK_PROBE_COMPLETE(ud,ns,name,src,rv,args) \
    ap_hook_probe_complete(ud, #name, src, (void *)rv, ap_hook_probe_arg1 args)

#define APR_HOOK_PROBE_RETURN(ud,ns,name,rv,args)

void *ap_hook_probe_entry(const char *name);
void ap_hook_probe_complete(void *ud, const char *name, const char *src, void *rv, const void *arg1);
const void *ap_hook_probe_arg1(const void *arg1, ...);
void ap_hook_probe_invoke(void *ud, const char *name, const char *src, const void *arg1);

#endif /* AP_HOOK_PROBES_H */
