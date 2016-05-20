   SCE CONFIDENTIAL
   PlayStation(R)3 Programmer Tool Runtime Library 400.001
   Copyright (C) 2007 Sony Computer Entertainment Inc.
   All Rights Reserved.


カスタムコントローラのサンプル


<サンプルの解説>
    このプログラムは、コントローラライブラリ(libpad)を用いて、
    カスタムコントローラでのシステムダイアログの操作を行うサンプルです。

<ファイル>
        SampleApp.cpp : カスタムコントローラのサンプル

<操作方法>
    サンプルを起動すると、タイムスケールに従い以下の一連の操作を行います。

    1. PSボタン押下をシステムに通知し、システムダイアログを開く
    2. 方向キー↓を送信
    3. 方向キー↑を送信
    4. PSボタン押下を通知し、システムダイアログを閉じる
    5. 1へ(繰り返し)

