.. -*- coding: utf-8; -*-

===========================
 Cプログラム用テンプレート
===========================

概要
====

Cプログラムを書くためのテンプレート。

ファイル構成
============

===========       ========================================
ファイル名        概要
===========       ========================================
main.c            引数処理とメインモジュールのテンプレート
main.h            main.cc用のヘッダファイル
libtrace/tool.c   使いそうなツール類
libtrace/tool.h   tool.c用のヘッダファイル
libtrace/trace.c  デバッグ用トレーサ
libtrace/trace.h  trace.c用のヘッダファイル
libtrace/wscript  ビルド用の設定ファイル
temp.c            その他のモジュール用のテンプレート
temp.h            その他のモジュール用のヘッダファイル
waf               ビルド用スクリプトwaf
wscript           ビルド用の設定ファイル
===========       ========================================

使い方
======

#. 追加するモジュール用に ``temp.c`` 、 ``temp.h`` をコピーする。
#. コピーを修正して追加モジュールを作る。
#. 追加したモジュールの初期化関数を ``main.c`` の ``global_init()`` に追加する。
#. 追加したモジュールの終了用関数を ``main.c`` の ``global_deinit()`` に追加する。
#. ``wscript`` を修正する。

   * 追加したモジュールファイルを ``bld.program`` の ``source`` に追加する。
   * ``APPNAME`` と ``VERSION`` を変更する。

#. 引数処理を変更する場合は、 ``main.c`` の ``arg_handler()`` を変更する

   * ``getopt()`` の引数で処理するオプションを追加する。

     * オプションの処理は ``switch`` の中に ``case`` を追加する。

   * 通常の引数は、 ``arg_handler`` の最後の ``switch`` の部分で引数の数を考慮した処理を追加する。

#. ``usage()`` を修正する。

#. 以下のようにしてビルドする。

.. code-block:: bash

   % ./waf configure
   % ./waf build

pthreadの利用に関して
---------------------

``pthread`` を使用する場合にはtracerはやや異なる動作をするため、 ``wscript`` 内の書き方で ``pthread.h`` をチェックすること。

Copyright, License
==================

Copyright (c) 2015, Kyushu Univ.

**DO NOT REDISTRIBUTE THIS PROGRAM NOR A PART OF THIS PROGRAM.**
