// $Id: re_define.h,v 1.2 2012/01/30 11:54:18 watanama Exp $
// *****************************************************************************
// File : re_define.h
//
// Copyright (c) 2011 Renesas Electronics Corp.
// 20-1, Josuihon-cho, 5chome, Kodaira-shi, Tokyo
// Japan
// All right reserverd.
//
// Author     : Masashi Watanabe @ Renesas
// Description: Define common define macro, enum and so on
// *****************************************************************************
#ifndef __RE_DEFINE_H
#define __RE_DEFINE_H

namespace vpcl
{

//
// Reset feature
//

// All of the models which support reset feature must use the following 
// enum value as an argumnet of throw and catch.
// ex) if (mIsReset) {
//         throw emRstException;
//     }
//     ...
//     catch(eResetException n) {
//         switch (n) {
//             case emRstException:
//     ...
enum eResetException {
    emRstException = 900,
};


// Active level of reset port should be evaluated with the following enum type.
// It is easy to update the reset port active level
// ex) if (rst.read() == emResetActive) {
//     ...
#ifdef IS_RESET_ACTIVE_LOW
enum eResetActiveLevel {
    emResetNonactive = 1,
    emResetActive    = 0,
};
#else
enum eResetActiveLevel {
    emResetNonactive = 0,
    emResetActive    = 1,
};
#endif

}; // namespae vpcl

#endif//__RE_DEFINE_H
