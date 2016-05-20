  SCE CONFIDENTIAL                                      
  PlayStation(R)3 Programmer Tool Runtime Library 400.001
  Copyright (C) 2009 Sony Computer Entertainment Inc.   
  All Rights Reserved.                                  


<サンプルの解説>
	本サンプルは、NP Clans ライブラリを紹介するサンプルです。

<ファイル>
    main.c					：本サンプルメイン処理。
    np_gui.cpp				：基本的な処理。コールバックの結果処理など。
    np_gui.h				：本サンプルメインヘッダ。
    np_gui_draw.h			：本サンプルの描画に関する定義。
    strings.h				：文字列定義。
    np_top.cpp				：トップ画面での処理。
    np_top_pad.cpp			：トップ画面でのパッド処理。
	netstart_dialog.cpp		：ネットスタートダイアログ処理。
	osk_dialog.cpp			：OSKダイアログ処理。
	np_clans.cpp			：クランモードのメイン処理。
	np_clans_draw.cpp		：クランモードの描画処理。
	np_clans_pad.cpp		：クランモードのパッド処理。
	np_gui_async_pad.cpp	：ブロックAPI呼び出し中の中断処理。

<動作説明>
    本サンプルを起動するとトップ画面が表示されます。

    本サンプルにおける◯ボタン、×ボタンの動作は、システムに設定されて
    いるボタン割り当てに一致します。以降の操作説明では、◯ボタン×ボタ
    ンのうち、システムに Enter と設定されているボタンを Enterボタン、
    もう一方を Back ボタンと表記します。

<トップ画面>
	"CLAN MODE" を選択し、クランモードに入ります。

    <操作方法>
    
	↑↓(方向キー)	：カーソル移動 (モード選択)
	Enterボタン		：決定

	トップ画面から各モードに遷移するには、NPサインインが完了しオンライ
	ン状態になっている必	要があります。NPオンラインでない場合は、ネッ
	トワーク開始ダイアログユーティリティが起動し、NPサインインを実行し
	ます。


<CLANS MODE>

	クランモードは大きく分けて、「クラン選択画面」と「クラン操作画面」
	の2つの操作画面に分かれています。「クラン選択画面」ではクランの
	作成および検索を行います。検索結果からクランを選択することで、その
	クランに対する処理を行う「クラン操作画面」へ遷移します。「クラン操作
	画面」では、キャンセルボタンを押すことで「クラン選択画面」へ戻る
	ことができます。

	<クラン選択画面>

		クラン選択画面で選択可能な操作は次の通りです。

		それぞれの操作の結果は一番右の列にリスト表示されます。
		リストからクランを選択して決定ボタンを押すと、「クラン操作画面」へ
		遷移します。

		CREATE CLAN
			クランを新たに作成します。
			この操作では、リスト表示は更新されません。
		GET CLAN LIST
			ユーザに関連するクランを取得します。
			TOTAL を実行すると、トータル数のみ取得します。GET を実行すると、
			リスト表示が更新されます。
		GET CLAN LIST BY NPID
			Np Id を指定して、他のユーザに関連するクランを取得します。
			基本的な動作は、GET CLAN LIST と同じです。
		SEARCH BY PROFILE
			クランプロファイルを元に、クランの検索を行います。検索結果には
			自分に関連しないクランも含まれます。
			各パラメータの直下には検索演算子が表示されています。検索結果
			に指定したくない場合は "---" を指定してください。
			TOTAL を実行すると、トータル数のみ取得します。SEARCH を実行すると、
			リスト表示が更新されます。
		SEARCH BY NAME
			クラン名を元に、クランの検索を行います。検索結果には
			自分に関連しないクランも含まれます。
			パラメータの直下には検索演算子が表示されています。
			TOTAL を実行すると、トータル数のみ取得します。SEARCH を実行すると、
			リスト表示が更新されます。

	<クラン操作画面>

		「クラン検索画面」で選択したクランに対して各種操作を行います。

		DISBAND
			選択したクランを解散します。
		GET CLAN INFO
			選択したクランの情報を表示します。
		UPDATE CLAN INFO
			選択したクランの情報を更新します。
			各パラメータを入力して、UPDATE を実行してください。
		GET AUTO ACCEPT STATUS
			選択したクランの自動許可状態を表示します。
		UPDATE AUTO ACCEPT STATUS
			選択したクランの自動許可状態を更新します。
			各パラメータを入力して、UPDATE を実行してください。
		GET MEMBER LIST
			選択したクランに関するメンバリストを取得します。
			リストの結果は一番右の列に表示されます。
			TOTAL を実行すると、トータル数のみ取得します。GET を実行すると、
			リスト表示が更新されます。
		GET MEMBER INFO
			選択したクランに関するメンバの NP ID を指定して、詳細なメンバ
			情報を取得します。
		UPDATE MEMBER INFO
			選択したクランでの自ユーザの情報を更新します。
			各パラメータを入力して、UPDATE を実行してください。
		CHANGE MEMBER ROLE
			選択したクランに関するメンバの NP ID を指定して、そのメンバの
			ROLE を変更します。
		JOIN CLAN
			選択したクランに参加します。
		LEAVE CLAN
			選択したクランから脱退します。
		SEND INVITATION REQ
			NP ID で指定したユーザに対して、選択したクランへの招待メッセージ
			を送信します。
		CANCEL INVITATION
			NP ID で指定したユーザに対して送信した招待メッセージをキャンセル
			します。
		SEND INVITATION RES
			自ユーザに送られた招待メッセージに対してレスポンスを送信します。
		SEND MEMBERSHIP REQ
			選択したクランへの参加要求メッセージを送信します。
		CANCEL MEMBERSHIP
			選択したクランへ送信した参加要求メッセージをキャンセルします。
		SEND MEMBERSHIP RES
			自クランに対して送られた参加要求メッセージに対してレスポンスを
			送信します。
		GET BLACKLIST
			選択したクランのブラックリストを取得します。
			TOTAL を実行すると、トータル数のみ取得します。GET を実行すると、
			リスト表示が更新されます。
		ADD BLACKLIST
			選択したクランのブラックリストへ、NP ID で指定したユーザを
			追加します。
		REMOVE BLACKLIST ENTRY
			選択したクランのブラックリストから、NP ID で指定したユーザを
			削除します。
		RETRIEVE ANNOUNCEMENTS
			選択したクランのアナウンスメントを取得します。
			TOTAL を実行すると、トータル数のみ取得します。RETRIEVE を実行
			すると、リスト表示が更新されます。
		POST ANNOUNCEMENTS
			選択したクランへアナウンスメントを送信します。
		REMOVE ANNOUNCEMENTS
			選択したクランから、ANNOUNCEMENTS ID で指定したアナウンスメントを
			削除します。
		POST CHALLENGE
			選択したクランからターゲットクランへチャレンジを送信します。
		RETRIEVE POSTED CHALLENGES
			選択したクランからターゲットクランに送信済みのチャレンジを
			取得します。
			TOTAL を実行すると、トータル数のみ取得します。RETRIEVE を実行
			すると、リスト表示が更新されます。
		REMOVE POSTED CHALLENGE
			選択したクランからターゲットクランに送信済みのチャレンジを
			CHALLENGE ID で指定して削除します。
		RETRIEVE CHALLENGES
			選択したクランが受信済みのチャレンジを取得します。
			TOTAL を実行すると、トータル数のみ取得します。RETRIEVE を実行
			すると、リスト表示が更新されます。
		REMOVE CHALLENGE
			選択したクランが受信済みのチャレンジを、CHALLENGE ID で
			指定して削除します。
