=begin

= SDL

すべてのクラス/モジュールはmodule SDLの下にあります。

== クラス/モジュール構成
* ((<Error>))
* ((<Surface>))
  * ((<Screen>))
* ((<PixelFormat>))
* ((<Event>))
* ((<Mixer>)) (module)
* ((<Mixer::Wave>))
* ((<WM>)) (module)
* ((<Key>)) (module)
* ((<Mouse>)) (module)

== Error

エラー通知用のクラスです。SDLモジュール内で生じるエラーほぼすべて
このクラスを用いて通知されます。

=== スーパークラス

StandardError

== video関連

=== SDL内のモジュール関数

--- blitSurface(src,srcX,srcY,srcW,srcH,dst,dstX,dstY)

--- warpMouse(x,y)

=== Surface

画像を保持するクラスです。

==== スーパークラス

Object

==== クラスメソッド

--- Surface.new(flag,w,h,format)

--- Surface.loadBMP(filename)

==== メソッド

--- Surface#mapRGB(r,g,b)

--- Surface#mapRGBA(r,g,b,a)

--- Surface#getRGB(pixel)

--- Surface#getRGBA(pixel)

--- Surface#displayFormat

--- Surface#setColorKey(flag,key)

--- Surface#fillRect(x,y,w,h)

--- Surface#setClipRect(x,y,w,h)

--- Surface#setAlpha(flag,alpha)

--- Surface#h

--- Surface#w

--- Surface#format

=== Screen

ここにかきこまれた画像が画面に表示されます。

==== スーパークラス

((<Surface>))

==== クラスメソッド

--- Screen.setVideoMode(w,h,bpp,flags)

==== メソッド

--- Screen#updateRect(x,y,w,h)

--- Screen#flip

=== PixelFormat

SDL_PixelFormatのラッパークラス。
Surface#formatで取得でき、((<Surface.new>))のフォーマットとしてこの
オブジェクトを与えることができる。

==== スーパークラス

Object

==== クラスメソッド

なし

==== メソッド

---PixelFormat#MapRGB(r,g,b)

---PixelFormat#MapRGBA(r,g,b,a)

---PixelFormat#getRGB(pixel)

---PixelFormat#getRGBA(pixel)

== Event関連

=== Event

イベントをとりあつかうクラス

==== スーパークラス

Object

==== クラスメソッド

--- Event.new

==== メソッド

--- Event#poll

--- Event#type

--- Event#keyPress?

--- Event#keySym

--- Event#keyMod


--- Event#gain?

--- Event#appState

--- Event#mouseX

--- Event#mouseY

--- Event#mouseXrel

--- Event#mouseYrel


--- Event#mouseButton

--- Event#mousePress?

=== Key

キーボードのキーに対応する定数を定義しているモジュール
キーボードのキーの状態を知るための関数もこの中にある。

==== モジュール関数

--- scan
      キーボードの状態をスキャンします

--- state(key)
      これを呼び出す前にscanを呼びだしてください。
      scan で得た状態を得ます。1で押している、0で離している、です。

--- modState

--- enableKeyRepeat(delay,interval)

--- disableKeyRepeat

== Mouse

マウス関連の定数と関数を定義しているモジュール

=== モジュール関数

--- state
      マウスの状態を配列の形で返します。
      配列の内容は

      [ x , y , pressLButton? , pressMButton? , pressRButton? ]

      となっています。

== audio関連

=== Mixer

音をだしたりするのに使うモジュール
ボリュームは0から128が有効です。

==== モジュール関数

--- open(frequency,format,channels,chunksize)

--- spec

--- playChannel(channel,wave,looping)

--- play?(channel)

--- setVolume(channel,volume)

--- halt(channel)

--- pause(chennel)

--- resume(channel)

--- pause?(channel)

=== Mixer::Wave

Waveをあらわすクラス

==== スーパークラス

Object

==== クラスメソッド

--- Mixer::Wave.load(filename)

==== メソッド

--- Mixer::Wave#setVolume(volume)

== Window Manager 関連

=== WM

Window関連の処理をまとめたモジュール

==== モジュール関数

--- caption

--- setCaption(title,icon)

--- icon=(iconImage)

--- iconify
