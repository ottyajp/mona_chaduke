#AskMonaを利用するアプリケーションを作るぞ
専ブラじゃ専ブラじゃ〜〜〜〜


#どうやってAPI利用するの
##手順だ
1. アプリケーション登録する
2. シークレットキーを取得する
3. 認証キーを取得する
4. APIを使う

###アプリケーション登録
済んでる
- アプリケーションID： 2332
- 開発者シークレットキー： Av610r8WvmpW4Vz3KseGRIF/SJpQFvP1Ul4EXgCZb7qQ=
- アプリ連携ページ： http://askmona.org/auth/?app_id=2332

###シークレットキーを取得する
シークレットキー取得APIがある。っょぃ

```
POST http://askmona.org/v1/auth/secretkey
```

- 送信する方
| パラメータ | 説明 |
|--------|--------|
|app_id|開発者のユーザID（2332）|
|app_secretkey|開発者シークレットキー（Av610r8WvmpW4Vz3KseGRIF/SJpQFvP1Ul4EXgCZb7qQ=）|
|u_address|利用者のMonacoinアドレス（M9MVFihH7VBAUciXg1BpbaqfXnHMUYfvtz）|
|pass|利用者のパスワード（S1419866730E）|

- 受信するもの
|レスポンス|説明|
|-|-|
|status |結果。1は成功、0は失敗|
|error (string)|エラーの場合の追加情報|
|u_id |利用者のユーザーID|
|secretkey (string)|認証キーの作成に使うシークレットキー|

```
"{"status":1,"u_id":2332,"secretkey":"UWIjYQpTWD3zFMS4tHf0PzDG2ttFywoquan\/vi4hmZyE="}"
```

###認証キーを取得する
つーか作成するんだけどね

```
開発者シークレットキー、nonce、time、認証キーの作成に使うシークレットキーの順に連結した文字列をSHA-256でハッシュ化し、そのバイナリデータをBase64でエンコードしたもの
```

- 開発者シークレットキー：`$app_secretkey`
- `nonce`:ランダムな文字列。リクエスト毎に変更する
- `time`：`nonce`が生成された時刻（UNIX時刻）

```php
//phpのサンプルコード
$app_secretkey = 'Av610r8WvmpW4Vz3KseGRIF/SJpQFvP1Ul4EXgCZb7qQ=';
$secretkey = 'SecretKey取得APIで貰ったもの';
$nonce = base64_encode(mcrypt_create_iv(32,MCRYPT_DEV_URANDOM));
$time = time();
//$auth_keyが認証キーです。
$auth_key = base64_encode(hash('sha256',$app_secretkey.$nonce.$time.$secretkey,TRUE));
```

- PHP嫁ねーよks
- http://php.net/manual/en/function.mcrypt-create-iv.php

###APIを使う
あとは各APIのリファレンスに従え


#使いそうなデータをリストアップしよう
機能ごとに分けて行ったほうが良さそう。つか最早リファレンスそのものだよね感がすごい

特に断りがなければ`integer`です

文字列は`\u304a\u8336\u6f2c\u3051\u3055\u3089\u3055\u3089`とかなってて困ったちゃんなのでUTF-16です（いみわかんねぇ）http://so-zou.jp/web-app/text/encode-decode/

##トピック一覧
###送るもの

|||
|-|-|
|limit|取得するトピックの個数（<=1000）|

###受け取るもの

|||
|-|-|
|status|1なら成功|
|error(string)|エラーの場合の追加情報|
|topics(list)|トピックそのもの〜|

###トピックオブジェクト（`topics(list)`）
|||
|-|-|
|rank|トピックの順位（トピックを単体で取得した場合には返ってこない|
|t_id|トピックID|
|title(string)|タイトル|
|cat_id|カテゴリ|
|category(string)|カテゴリ|
|tags(string; nullable)|タグ|
|lead(string)|リード文|
|created|作成時刻(UNIX時刻)|
|updated|最後にトピックが浮上した時刻(UNIX時刻)|
|modified|更新された時刻(UNIX時刻)|
|count|レス数|
|receive(string)|やりとりされたMONA数（watanabe単位）|
|favorites|お気に入り登録者数|

##トピック中身表示
レス取得APIを使う

###送るもの
|||
|-|-|
|t_id|トピックID|
|from|取得するレス番号の開始位置（デフォは1、minは1）|
|to|取得するレス番号の終了位置（省略した場合はfromで指定した1レスのみ取得）|
|if_modified_since|前回問い合わせ時のUNIX時刻を指定する。更新がなければstatusに2が返ってくる|

```
GET http://askmona.org/v1/responses/list?t_id=2778&from=1&to=3
```

###受け取るもの
|||
|-|-|
|status|1:成功、0:失敗、2:更新なし|
|error(string)|エラーの場合の追加情報|
|modified|更新された時刻(UNIX時刻)|
|responses(object)|レスポンスオブジェクトのリスト|

```
{"status":1,"updated":1431999626,"modified":1431999626,"responses":[{"r_id":1,"created":1421917742,"u_id":438,"u_name":"\u540d\u524d\u3092\u304f\u3060\u3055\u3044\u3002","u_dan":"\u4e94\u6bb5","u_times":"1\/13","receive":"114114064","res_lv":3,"rec_count":4,"response":"\u7d75\u3092\u63cf\u3044\u3066\u307f\u307e\u3057\u3087\u3046\u3002MONA\u304c\u3082\u3089\u3048\u308b\u304b\u3082\u3057\u308c\u307e\u305b\u3093\u3002\n\u30b5\u30e0\u30cd\u30a4\u30eb\u8868\u793a\u306fimgur\u3067\u3044\u3051\u308b\u3089\u3057\u3044\u3067\u3059\u3002\n\n\u524d\u30c8\u30d4\nhttp:\/\/askmona.org\/3?n=1000"},{"r_id":2,"created":1421939525,"u_id":1255,"u_name":"Urara","u_dan":"\u516d\u6bb5","u_times":"1\/2","receive":"4219852173","res_lv":5,"rec_count":13,"response":"\u30a2\u30cb\u5a18\u3061\u3083\u3093\u3002\n\u5857\u308a\u59cb\u3081\u308b\u307e\u3067\u304c\u9577\u3044\u3063\uff01\u3000\u4e09\u30f6\u6708\u9045\u308c\u30fc\nhttp:\/\/i.imgur.com\/16hEmMw.jpg"},{"r_id":3,"created":1421947388,"u_id":438,"u_name":"\u540d\u524d\u3092\u304f\u3060\u3055\u3044\u3002","u_dan":"\u4e94\u6bb5","u_times":"2\/13","receive":"0","res_lv":0,"rec_count":0,"response":"\u304b\u308f\u3048\u3048\u306a\n\u5857\u3082\u7dba\u9e97\u3060"}]}
```

###レスポンスオブジェクト
|||
|-|-|
|r_id|レス番号|
|created|投稿された時刻(UNIX時刻)|
|u_id|投稿した人のユーザID|
|u_name(string)|投稿した人の名前|
|u_dan(string)|投稿した人の段位|
|u_times(string)|投稿回数。5回中3回めの投稿の場合`3/5`|
|receive(string)|やり取りされたMONA（watanabe単位）|
|res_lv|レスレベル（MONAでわけられる）。`0 <= res_lv <= 7`|
|res_count|そのレスにMONAを送った人数|
|response(string)|レス本文|

##ユーザプロフィール表示
###送るもの

|||
|-|-|
|u_id|ユーザID|

```
http://askmona.org/v1/users/profile?u_id=2332
```

###受け取るもの

|||
|-|-|
|status|1:成功、0:失敗|
|error(string)|エラーの場合の追加情報|
|u_name(string)|ユーザの名前|
|u_dan(string)|ユーザの段位|
|profile(string; nullable)|ユーザのプロフィール|

```
{"status":1,"u_name":"\u304a\u8336\u6f2c\u3051\u3055\u3089\u3055\u3089","u_dan":"\u516b\u6bb5","profile":"\u51cd\u7d50\u3055\u308c\u3066\u6ce3\u3044\u3066\u308b\n"}
```

##投稿
##送金
##設定できるべき項目
###認証関係
|||
|-|-|
|u_address|利用者のMonacoinアドレス|
|pass|利用者のパスワード|
|u_id（見えるだけにしておく）|利用者のユーザID|

###トピック取得系
###レス取得系
###送金関係



















