/**
 * Copyright 2014 George C. Hawkins
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
#ifndef PRINT_ACI_INFO_H_
#define PRINT_ACI_INFO_H_

#include <lib_aci.h>

extern void printAciInfo(aci_state_t& aci_state, aci_evt_t& aci_evt);
extern void printByteArray(const uint8_t* data, size_t len);

#endif /* PRINT_ACI_INFO_H_ */
