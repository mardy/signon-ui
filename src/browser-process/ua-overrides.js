/*
 * Copyright 2013 Canonical Ltd.
 *
 * This file is part of webbrowser-app.
 *
 * webbrowser-app is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * webbrowser-app is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

.pragma library

// B2G’s list of overrides: https://github.com/mozilla-b2g/gaia/blob/master/build/ua-override-prefs.js

// List of user agent string overrides in the form of an object.
// Each key is a domain name for which the default user agent string doesn’t
// work well enough. Values can either be a string (full override) or an array
// containing two values that are passed to the String.replace method (the
// first value may be either a string or a regular expression, the second value
// must be a string).

// Examples of valid entries:
//     "example.org": "full override"
//     "example.com": ["Ubuntu", "Ubuntu Edge"]
//     "google.com": [/mobi/i, "b"]

// The original list was initially built from the top 100 entries
// at http://www.alexa.com/topsites (2013-08-16), using Chrome on
// Android as a reference.

var overrides = {
    "mail.google.com": [/Mobile\) WebKit\/[.0-9]*/, "Android 4.3) AppleWebKit Mobile Safari"],
    "google.com": ["Mobile", "Android; Mobile"],
    "youtube.com": ["Mobile", "Android; Mobile"],
    "yahoo.com": ["Mobile", "Android; Mobile"],
    "baidu.com": ["Mobile", "Android; Mobile"],
    "qq.com": [/WebKit\/[.0-9]*/, "Apple$& Mobile"],
    "amazon.com": ["Mobile", "Android; Mobile"],
    "linkedin.com": ["Mobile", "Android; Mobile"],
    "blogspot.com": ["Mobile", "Android; Mobile"],
    "taobao.com": ["Mobile", "Android; Mobile"],
    "google.co.in": ["Mobile", "Android; Mobile"],
    "bing.com": ["Mobile", "Android; Mobile"],
    "yahoo.co.jp": ["Ubuntu", "Linux; Android 4; Galaxy Build/"],
    "yandex.ru": ["Mobile", "Android; Mobile"],
    "sina.com.cn": ["Mobile", "Android; Mobile"],
    "ebay.com": ["Mobile", "Android; Mobile"],
    "google.de": ["Mobile", "Android; Mobile"],
    "tumblr.com": ["Mobile", "Android; Mobile"],
    "google.co.uk": ["Mobile", "Android; Mobile"],
    "msn.com": ["Mobile", "Android; Mobile"],
    "google.fr": ["Mobile", "Android; Mobile"],
    "mail.ru": ["Ubuntu", "Linux; Android 4; Galaxy Build/"],
    "google.com.br": ["Mobile", "Android; Mobile"],
    "google.co.jp": ["Mobile", "Android; Mobile"],
    "hao123.com": ["Mobile", "Android; Mobile"],
    "ask.com": ["Mobile", "Android; Mobile"],
    "google.com.hk": ["Mobile", "Android; Mobile"],
    "google.ru": ["Mobile", "Android; Mobile"],
    "blogger.com": ["Mobile", "Android; Mobile"],
    "imdb.com": ["Mobile", "Android; Mobile"],
    "google.it": ["Mobile", "Android; Mobile"],
    "google.es": ["Mobile", "Android; Mobile"],
    "amazon.co.jp": ["Mobile", "Android; Mobile"],
    "tmall.com": ["Mobile", "Android; Mobile"],
    "fc2.com": ["Mobile", "Android; Mobile"],
    "google.com.mx": ["Mobile", "Android; Mobile"],
    "google.ca": ["Mobile", "Android; Mobile"],
    "soso.com": ["Mobile", "Android; Mobile"],
    "delta-search.com": ["Mobile", "Android; Mobile"],
    "odnoklassniki.ru": ["Mobile", "Android; Mobile"],
    "alibaba.com": ["Mobile", "Android; Mobile"],
    "flickr.com": ["Mobile", "Android; Mobile"],
    "amazon.de": ["Mobile", "Android; Mobile"],
    "blogspot.in": ["Mobile", "Android; Mobile"],
    "ifeng.com": ["Mobile", "Android; Mobile"],
    "360.cn": ["Mobile", "Android; Mobile"],
    "google.com.tr": ["Mobile", "Android; Mobile"],
    "google.com.au": ["Mobile", "Android; Mobile"],
    "youku.com": ["Mobile", "Android; Mobile"],
    "ebay.de": ["Mobile", "Android; Mobile"],
    "uol.com.br": ["Mobile", "Android; Mobile"],
    "aol.com": ["Mobile", "Android; Mobile"],
    "google.pl": ["Mobile", "Android; Mobile"],
    "alipay.com": ["Mobile", "Android; Mobile"],
    "dailymotion.com": ["Mobile", "Android; Mobile Safari"],
    "amazon.co.uk": ["Mobile", "Android; Mobile"],
    "ebay.co.uk": ["Mobile", "Android; Mobile"],
    "facebook.com": [/WebKit\/[.0-9]*/, "Apple$& Firefox/18"],
    "nytimes.com": ["Mobile", "Android; Mobile Safari"],
};
