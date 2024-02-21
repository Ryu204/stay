#pragma once

#define CRTP_UNDERLYING(T) \
T& underlying() { return static_cast<T&>(*this); }\
const T& underlying() const { return static_cast<const T&>(*this); }
