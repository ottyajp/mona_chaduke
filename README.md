#モナ茶漬け

さらさら


AskMonaのクライアントをプログラミング初心者がじわじわ作ります。




##今出来ること
- トピックリスト取得、トピック取得、投稿する（sage可）、レス相手にMONA送金する
- imgurの画像をサムネ表示、クリックで拡大表示する
- お気に入り機能を取り扱う
- トピックの中身をローカルに保存して鯖負荷軽減
- 残高を見る
- 任意のIDの人のプロフィールを見る
- 入金アドレスを表示する
- 出金する
- 入出金の履歴を見る

##これから実装したいこと（思いついた順）
- プロフィール変更出来るように
- ユーザ相手に送金出来るようにする
- トピックの作成
- 非匿名送金（メッセージつけて送金）
- URL見つけたらブラウザで開くようにリンクする
- アンカーに対応したい
- ばらまき、受け取りの履歴を見る
- どのユーザでログインしているのか、IDでなく名前で表示するようにしたい

##使い方
Qt開発環境があれば、ローカルにcloneして貰ってビルドすれば良いと思います。

###初期設定
初期値設定をあまりしていないので、最初に設定してください。設定しないままだと多分まともに使えません。

まずファイルメニューの下にいるコンフィグを開きます。`Alt+F->Alt+C`みたいな感じ

空欄を適宜埋めていきます。要認証機能（送金や投稿など）を使う場合はモナコインアドレスとパスワードを入力し、`Secret_KeyとIDを取得`ボタンを押します。認証に成功すればユーザIDがID欄に入ります。失敗した場合はやり直して下さい（パスワード間違いの場合、AskMona側でカウントされていくので凍結されないよう気を付けてください）

なお、モナコインアドレスとパスワードはモナ茶漬け側で保存していません（ユーザIDとシークレットキーを保存しています）。現在どのアカウントでログイン（認証）しているかというのは、コンフィグ画面に書いてあるIDをプロフィールメニューの下のプロフィールを取得（`Alt+P->Alt+G`みたいな感じ）に入力し確認してください。

怖くなったら（？）AskMona本家サイトのマイページから認証を取り消してください。Secret_Keyが無効になるのでモナ茶漬けからはどうする事もできなくなります

`トピックリスト取得上限`はトピックリストを一度に取得する数、~~`レス取得上限`はレスを取得する数です。少なくともレス取得上限は将来的に廃止予定です（トピックをローカルに保存するように出来た時点で）。~~廃止しました

`送るMONA量のプリセット`はAskMonaサイトの`ワンクリックでMONA送る`ボタンの中途半端実装です。好きな値をプリセットとして設定してください。

###トピックを表示
メニューで、`トピック→トピックリストを取得`すると、メインウィンドウ上部にリストが現れます。

これをダブルクリック/選択した状態でEnterするとロードして、適宜整形してメインウィンドウ下部に表示します。

imgurの画像がレスの中に貼られていた場合、サムネイルを表示し、それをクリックすると拡大して表示する事が出来ます。ただ、稀にうまく表示出来ずトピックの表示もすっ飛ぶ事があります。（原因は調べてない）

後述するように、トピックの中身（レス達）はログとして保存していますが、すっ飛んだ状態でログとして保存してしまうと面倒な事になるので、その時用に`トピック→強制的にリロード`が用意してあります。ログを破棄して再度取得します。

####トピックのログ
トピックを取得すると、ローカルに`log`とかいうファイルが出現します。ここに取得したトピックがそっくりそのまま保存されているので、迂闊にテキストエディタで開くと大変な事になるかもしれません。

ログが肥大化して邪魔くせーなと思うなら消してくれても良いですが、サーバ負荷軽減に繋がっているみたいなので残しておいてくれた方が良いと思います。ちなみに2015/06/18の13時過ぎの時点で、頭から100個ほどログ拾ってみましたが4.5MB程になりました。案外少ないですね。

###お気に入り関係
- 追加：トピックを表示している状態で追加しそうなボタンを押せば良いです
- 削除：トピック表示してなくてもリストで選択した状態で削除できます
- AskMonaのお気に入りAPI使ってるのでそれと連動してます

##え？
寄付くれるんですか？大歓迎ですよ♡

```
M9MVFihH7VBAUciXg1BpbaqfXnHMUYfvtz
```
