/*
    Copyright (C) 2013 by Maxim Biro <nurupo.contributions@gmail.com> Emmanuelduv <emmanuelduviviers49@hotmail.com>
    This file is part of Tox Qt GUI and modified to compile with older GCC (without good C++ 11).
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
    See the COPYING file for more details.
*/

#ifndef CDATA_H
#define CDATA_H

#if __cplusplus >= 201103L
 #include <cstdint> //C++ 11
#else
 #include <stdint.h> //C++ < 11 (<=> GCC < 4.8)
#endif

class QString;
class CData
{
public:
    uint8_t* data();
    uint16_t size() const;

protected:
    explicit CData(const QString& data, uint16_t byteSize);
    CData(const CData& other)=delete;
    virtual ~CData();
    CData& operator=(const CData& other)=delete;

    static QString toString(const uint8_t* cData, const uint16_t cDataSize);

private:
    uint8_t* cData;
    uint16_t cDataSize;

    static uint16_t fromString(const QString& userId, uint8_t* cData);
};

class CUserId : public CData
{
public:
    explicit CUserId(const QString& userId);

    static QString toString(const uint8_t *cUserId);

private:
    static const uint16_t SIZE;

};

class CFriendAddress : public CData
{
public:
    explicit CFriendAddress(const QString& friendAddress);

    static QString toString(const uint8_t* cFriendAddress);

private:
    static const uint16_t SIZE;

};

#endif // CDATA_H
