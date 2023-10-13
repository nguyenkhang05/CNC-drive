/*
 * (c) 2011,2012,2013,2014,2015,2016,2017,2018 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */
#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

/** \file softfloat_test_common.h
 * \brief The header file contains the common function for SoftFloat UT development.
 *
 * This header file contains the common functions for SoftFloat UT development.
 *    - 2 functions for conversion from string to unsigned integer value.
 *    - 2 functions for parsing data.
 */


/** Parse each field to many token by specified delimit character.
 * @param[in] str
 * @param[in] delim
 * @return tokens
 * @retval token in one field
 * @todo To be documented.
 */
std::vector<std::string> split(const std::string &str, const std::string &delim) {
  std::vector<std::string> tokens;
  size_t cp, fp; // current position, found position
  const size_t dsize = delim.size();
  for (cp = 0; (fp = str.find(delim, cp)) != std::string::npos; cp = fp + dsize) {
    tokens.emplace_back(str, cp, fp - cp);
  }
  tokens.emplace_back(str, cp, str.size() - cp);
  return tokens;
}


/** Parse each line to many fields by specified delimit character.
 * @param[in] stream
 * @param[in] delim
 * @return tbl
 * @retval fields in one line
 * @todo To be documented.
 */
std::vector<std::vector<std::string>> parseTable(std::istream &stream, const std::string &delim) {
  std::vector<std::vector<std::string>> tbl;
  std::string str;
  while (getline(stream, str)) {
    tbl.push_back(split(str, delim));
  }
  return tbl;
}


/** Conversion from string to uint16_t value
 * @param[in] str
 * @return res
 * @retval unsigned int 16bit value
 * @todo To be documented.
 */
uint16_t s2ul_16(std::string str) {
  std::string prefix = str.substr(0, 2);
  int n = 10;
  int st = 0;
  // hexadecimal
  if (prefix == "0x") {
    n = 16;
    st = 2;
  }

  std::string data = str.substr(st, str.length() - st);
  uint16_t res = strtoul(data.c_str(), 0, n);

  return res;
}


/** Conversion from string to uint32_t value
 * @param[in] str
 * @return res
 * @retval unsigned int 32bit value
 * @todo To be documented.
 */
uint32_t s2ul_32(std::string str) {
  std::string prefix = str.substr(0, 2);
  int n = 10;
  int st = 0;
  // hexadecimal
  if (prefix == "0x") {
    n = 16;
    st = 2;
  }
  // binary number
  if (prefix == "0b") {
    n = 2;
    st = 2;
  }
  std::string data = str.substr(st, str.length() - st);
  uint32_t res = strtoul(data.c_str(), 0, n);
  return res;
}


/** Conversion from string to uint64_t value
 * @param[in] str
 * @return res
 * @retval unsigned int 64bit value
 * @todo To be documented.
 */
uint64_t s2ull_64(std::string str) {
  std::string prefix = str.substr(0, 2);
  int n = 10;
  int st = 0;
  // hexadecimal
  if (prefix == "0x") {
    n = 16;
    st = 2;
  }

  std::string data = str.substr(st, str.length() - st);
  data.erase(4,1);
  data.erase(8,1);
  data.erase(12,1);
  uint64_t res = strtoull(data.c_str(), 0, n);
  return res;
}

