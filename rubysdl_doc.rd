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
* ((<TTF>))

== Error

エラー通知用のクラスです。SDLモジュール内で生じるエラーほぼすべて
このクラスを用いて通知されます。

=== スーパークラス

StandardError

== 初期化関連

=== SDL内のモジュール関数

--- init(flag)

--- quit
      at_exit{ SDL.quit; } というように使ってください。

--- initializedSystem(flag)

== video関連

一部の機能は、SGEライブラリが必要です。

=== SDL内のモジュール関数

--- blitSurface(src,srcX,srcY,srcW,srcH,dst,dstX,dstY)

--- warpMouse(x,y)

--- rotateXYScaled(src,dst,x,y,angle,xscale,yscale)
      SGEが必要
      SGEのsge_rotate_xyscaled関数とはsrcとdstの順序が入れ替わっていることに
      注意するように。下の2つの関数も同様である。
      これは、blitSurfaceに合わせたためである。
      また、この仕様は変更する可能性がある。

--- rotateScaled(src,dst,x,y,angle,scale)

--- rotate(src,dst,x,y,angle)

=== Surface

画像を保持するクラスです。

==== スーパークラス

Object

==== クラスメソッド

--- Surface.new(flag,w,h,format)

--- Surface.loadBMP(filename)

==== メソッド

--- Surface#displayFormat

--- Surface#setColorKey(flag,key)

--- Surface#fillRect(x,y,w,h)

--- Surface#setClipRect(x,y,w,h)

--- Surface#setAlpha(flag,alpha)

--- Surface#h

--- Surface#w

--- Surface#format

--- Surface#put(image,x,y)

--- Surface#getPixel(x,y) 
--- Surface#[]
      SGEが必要

--- Surface#putPixel(x,y)
--- Surface#[]=
      SGEが必要

--- Surface#drawLine(x1,y1,x2,y2,color)
      SGEが必要

--- Surface#drawRect(x,y,w,h,color)
      SGEが必要

--- Surface#drawCircle(x,y,r,color)
      SGEが必要

--- Surface#drawFilledCircle(x,y,r,color)
      SGEが必要

--- Surface#rotateScaledSurface(angle,scale,bgcolor)
      SGEが必要
      これは、selfをangle度傾け、scale倍した画像を持つSurfaceのインスタンスを
      生成するメソッドである。回転によって生じる隙間はbgcolorで埋められる。

--- Surface#rotateSurface(angle,bgcolor)
      
=== Screen

ここにかきこまれた画像が画面に表示されます。
このクラスはただ一つしか生成できないようになっていて、
SDL::setVideoModeによってのみ生成される。
実際には、このようなクラスは存在せず、Surfaceに以下の特異メソッドを追加した
オブジェクトである。

==== スーパークラス

((<Surface>))

==== クラスメソッド


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
このモジュールの機能を使うためには、SDL_mixerライブラリが必要です。

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

== フォント関係

=== TTF

ＴｒｕｅＴｙｐｅフォントを表すクラス

==== スーパークラス

Object

==== クラスメソッド

--- TTF.init

--- TTF.open(filename,size)

==== メソッド

--- TTF#style

--- TTF#style=(style)

--- TTF#drawSolidUTF8(dest,text,x,y,r,g,b)
      selfのフォント設定でdest(Surfaceのインスタンス)にString textを
      destの位置(x,y)の所に書きこむ。色はr,g,bで決められる。
      透明色(ColorKey)は有効である。textはUTF-8を使う。
=end
