   SCE CONFIDENTIAL
   PlayStation(R)3 Programmer Tool Runtime Library 400.001
   Copyright (C) 2007 Sony Computer Entertainment Inc.
   All Rights Reserved.


ホストPCからのプログラム起動サンプル


<サンプルの解説>
    このプログラムは、Debugging Station に接続したホストPCに置いてある
    プログラム( /app_home/EBOOT.BIN )の起動を行うサンプルです。

<ファイル>
    main.cpp        : ホストPCからのプログラム起動サンプルのソースファイル
    main.h          : ホストPCからのプログラム起動サンプルのヘッダーファイル
    launcher.gp3    : Disc Image Generator for PLAYSTATION(R)3 のプロジェクトファイル
    launcher.conf   : NPDRM PACKAGE 設定ファイル
    PS3_GAME/       : パッケージデータが格納されるディレクトリ

<操作方法>
    プログラムを起動すると、/app_home/EBOOT.BIN というファイルにアクセスできるか
    チェックし、ファイルが存在すれば、3秒間待った後 exitspawn(...) で
    プログラムを起動します。
    ホストPCの /app_home にアクセスするには、
    ホストPCの dtnetm/dtcfilesv または ProDG Target Manager を
    Debugging Station に接続しておく必要があります。
    
    EBOOT.BINファイルが存在しない、もしくはアクセスできない場合、
    Debugging Station のIPアドレス情報と dtnetm で接続されている
    ホストPCのIPアドレスを表示し、ファイルにアクセスできるようになるまで待機します。

<launcher_args.txtについて>
    プログラムを起動すると、/app_home/launcher_args.txt というファイルを読み込みます。
    launcher_args.txt には起動プログラムのファイル名と、起動時の引数をスペース区切りで
    指定します。
    ファイル名や起動引数にスペースが入る場合は "" でその文字列をくくります。

    例1) /app_home/EBOOT.BIN を起動し、起動引数に a, b を渡します。
    launcher_args.txt:
      EBOOT.BIN a b

    例2) /app_home/myprogram.self を起動し、起動引数に "a b", c を渡します。
    launcher_args.txt:
      myprogram.self "a b" c

<ディスクイメージの作成方法>
    $ make gen_image を実行し、launcher.gp3 を Disc Image Generator for PLAYSTATION(R)3 で
    開き、ディスクイメージを作成します。
    作成済みの DVD-SL ISO イメージは
      $CELL_SDK/sample_data/util/debug/launcher/laucher-DVD-SL.iso.zip
    にあります。

<NPDRM PACKAGE の作成方法>
    $ make npdrm_package を実行すると、
    UP0001-SXXX00001_00-0000111122223333.pkg というパッケージファイルが生成されます。
    このパッケージファイルを利用し、Debugging Station の HDD にこのプログラムを
    インストールします。
    作成済みの NPDRM PACKAGE は
      $CELL_SDK/sample_data/util/debug/launcher/UP0001-SXXX00001_00-0000111122223333.pkg
    にあります。

