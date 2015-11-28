#モナ茶漬け

さらさら


AskMonaのクライアントをプログラミング初心者がじわじわ作ります。




##今出来ること
- トピックリスト取得、トピック取得、投稿する（sage可）、レス相手にMONA送金する
- imgurの画像をサムネ表示、クリックで拡大表示する
- アンカーに対応（暫定）
- お気に入り機能を取り扱う
- トピックの中身をローカルに保存して鯖負荷軽減
- 残高を見る、入金アドレスを表示する、出金する、入出金履歴を見る
- ばらまき、受け取りの履歴を見る（レス、ユーザ、メッセージ等を確認可）
- 任意のIDのユーザのプロフィールを見る、送金する
- 非匿名送金（メッセージつけて送金）
- ログイン中のユーザをわかりやすく表示する
- トピックの作成

##これから実装したいこと（思いついた順）
- プロフィール変更出来るように
- URL見つけたらブラウザで開くようにリンクする

##使い方
Qt開発環境があれば、ローカルにcloneして貰ってビルドすれば良いと思います。

###初期設定
初期値設定をあまりしていないので、最初に設定してください。設定しないままだと多分まともに使えません。

まずファイルメニューの下にいるコンフィグを開きます。`Alt+F->Alt+C`みたいな感じ

空欄を適宜埋めていきます。要認証機能（送金や投稿など）を使う場合はモナコインアドレスとパスワードを入力し、`Secret_KeyとIDを取得`ボタンを押します。認証に成功すればユーザIDがID欄に入ります。失敗した場合はやり直して下さい（パスワード間違いの場合、AskMona側でカウントされていくので凍結されないよう気を付けてください）

認証済みの場合、不用意に`Secret_KeyとIDを取得`ボタンを押してしまわないように無効化しています。それでも認証したい場合には、アドレスかパスワードの入力欄をいじると有効化されます。

なお、モナコインアドレスとパスワードはモナ茶漬け側で保存していません（ユーザIDとシークレットキーを保存しています）。現在どのアカウントでログイン（認証）しているかは、アドレス、パスワードの下のID欄に表示しています。

怖くなったら（？）AskMona本家サイトのマイページから認証を取り消してください。Secret_Keyが無効になるのでモナ茶漬けからはどうする事もできなくなります

`トピックリスト取得上限`はトピックリストを一度に取得する数です。

入出金履歴取得数、ばらまき/受取履歴取得数はそれぞれの取得数です。ここに設定した数のやり取りをそれぞれ取得します。

`送るMONA量のプリセット`はAskMonaサイトの`ワンクリックでMONA送る`ボタンの中途半端実装です。好きな値をプリセットとして設定してください。

なお、設定は本体近くにmona_chaduke.iniという名前でそのまま保存されています。

###トピックを表示
メニューで、`トピック→トピックリストを取得`すると、メインウィンドウ上部にリストが現れます。

これをダブルクリック/選択した状態でEnterするとロードして、適宜整形してメインウィンドウ下部に表示します。

imgurの画像がレスの中に貼られていた場合、サムネイルを表示し、それをクリックすると拡大して表示する事が出来ます。ただ、稀にうまく表示出来ずトピックの表示もすっ飛ぶ事があります。（原因は調べてない）

後述するように、トピックの中身（レス達）はログとして保存していますが、すっ飛んだ状態でログとして保存してしまうと面倒な事になるので、その時用に`トピック→強制的にリロード`が用意してあります。該当トピック（その時表示中）のログを破棄して再度取得します。

####トピックのログ
トピックを取得すると、ローカルに`log`とかいうファイルが出現します。ここに取得したトピックがそっくりそのまま保存されているので、迂闊にテキストエディタで開くと大変な事になるかもしれません。

ログが肥大化して邪魔くせーなと思うなら消してくれても良いですが、サーバ負荷軽減に繋がっているみたいなので残しておいてくれた方が良いと思います。ちなみに2015/06/18の13時過ぎの時点で、頭から100個ほどログ拾ってみましたが4.5MB程になりました。案外少ないですね。

###お気に入り関係
- 追加：トピックを表示している状態で追加しそうなボタンを押せば良いです
- 削除：トピック表示してなくてもリストで選択した状態で削除できます
- AskMonaのお気に入りAPI使ってるのでそれと連動してます

###入出金、ばらまき、受取の履歴について
たまに一部読み込みされない事があります、更新ボタンを活用してください。

####ばらまき、受取の履歴について
各やりとりをダブルクリックすると、詳細を表示できます。

1. レス相手の送金
  - そのレスを表示します
  - メッセージがあればメッセージも表示します
2. ユーザ相手の送金
  - 匿名送金でなければユーザプロフィールを表示します
  - 匿名送金だと表示できる情報が何もないので何も表示しません

###テンプレートの編集
トピックの整形時に使用する`template.html`を編集すると見た目を変更する事が出来ます。

javascriptの記述はプログラム本体の動作に関わる部分なのでいじらないでください。思いついた時にこのファイルから取り除きます。

|||
|-|-:|
|div.title|トピックタイトル|
|div.responses|トピックの本文全体|
|div.response|レス1つ1つ|
|span.u_name|ユーザ名|
|span.mona_yay|MONAを送られた量|
|span.send_mona|MONAを送るボタン|
|span.anchor|アンカー|
|span.level[0-7]|送られたMONA量によって変化する表示|
|div.image|画像表示部分|
|div.imgur|imgur表示部分|

##え？
寄付くれるんですか？大歓迎ですよ♡

```
M9MVFihH7VBAUciXg1BpbaqfXnHMUYfvtz
```
