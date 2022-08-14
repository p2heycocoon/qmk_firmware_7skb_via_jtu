# これは何か
## hexフォルダ内のhexファイル
7skb、7sPro用のファームウェアです。

qmk公式 https://github.com/qmk/qmk_firmware の `keyboards/salicylic_acid3/7skb/keymaps/via` をベースに、
JISキーボード設定のWIN機でそのまま使用できるように
キーコードの変換処理を追加しました。

他のファームウェアと同様、Pro Micro Web Updater などで書き込めます。

## via_jtuフォルダ内のファイル
上記hexファイルのソースです。
qmk_firmware の `keyboards/salicylic_acid3/7skb/keymaps` の直下に
`via_jtu` が来るように配置して、
QMK MSYSで `make salicylic_acid3/7skb:via_jtu` をすることにより
上記hexファイルが作成されます。

`keymap.c` 内の `// 以下追加` から `// 追加ここまで` がベースから追加した内容なので、
他のキーボードにも同じことをすれば活用できるかもしれません。


# 特徴
## 良い面
- 上記の通り、JISキーボード設定のWIN機でそのまま使用できるようになります。
- VIA対応です。
- `KC_AT` など、シフト押下後を直接指定するキーコードにも対応しています。
- 変換対象キー（＠や’など）を押しっぱなしで、通常キーと同じ様に連続出力できます。
- 変換対象キーを押したままで、放す前に他のキーを押しても問題ありません。

## 悪い面
- MAC機など、WIN機以外でどうなるか未検証です。
- 変換対象キーをシフトキーといっしょに押したままで、変換対象キーを放す前にシフトキーを放すと別のキーコードが出力される場合があります。（なので、文字入力やプログラミングなどには問題ないと思いますが、ゲームには不向きかもしれません。）



# 謝辞
7skb、7sProの開発者であるサリチル酸様がqmk公式 https://github.com/qmk/qmk_firmware に公開された `keymap.c` をベースにさせていただきました。ありがとうございます。

変換処理はkoktoh様の https://github.com/koktoh/jtu_custom_keycodes を参考にさせていただきました。ありがとうございます。