=begin

= SDL

この拡張ライブラリのインターフェースはSDL本体と非常に似ています。
よってSDLのドキュメントが非常に参考になるでしょう。

すべてのクラス/モジュールはmodule SDLの下にあります。
このモジュール内では数はほぼすべて0から始まります。

== クラス/モジュール構成
* ((<SDL::Error>))
* ((<SDL::Surface>))
  * ((<SDL::Screen>))
* ((<SDL::PixelFormat>))
* ((<SDL::Event>))
* ((<SDL::Key>)) (module)
* ((<SDL::Mouse>)) (module)
* ((<SDL::Mixer>)) (module)
* ((<SDL::Mixer::Wave>))
* ((<SDL::Mixer::Music>))
* ((<SDL::WM>)) (module)
* ((<SDL::CD>))
* ((<SDL::Joystick>))
* ((<SDL::TTF>))

== SDL::Error

エラー通知用のクラスです。SDLモジュール内で生じるエラーほぼすべて
このクラスを用いて通知されます。

=== スーパークラス

StandardError

== 初期化関連

=== SDL内のモジュール関数

--- SDL.init(flag)
      SDLを初期化する。flagとして与えられる定数は以下のとおり。
        INIT_AUDIO
        INIT_VIDEO
        INIT_CDROM
        INIT_JOYSTICK

--- SDL.quit
      at_exit{ SDL.quit; } というように使ってください。

      version 0.3にて廃止。ライブラリから自動的によびだされる。

--- SDL.initializedSystem(flag)
      Not documented yet

== video関連

一部の機能は、SGEライブラリやSDL_imageが必要である。

bpp=bit per pixelである。

=== SDL内のモジュール関数

--- SDL.getVideoSurface
      Not documented yet

--- SDL.setVideoMode(w,h,bpp,flags)
      指定された画面の幅、高さ、bppの値でビデオモードを設定する。
      bppが0の場合、現在のディスプレイのbppの値が使用される。
      成功したときは((<SDL::Screen>))のオブジェクトを返す。
      失敗したときはSDL::Error例外が生じる。
      flagsの意味は以下のとおり。
      * SDL::SWSURFACE
        
        システムのメモリ内にバッファをとる。

      * SDL::HWSURFACE

        ビデオメモリにバッファをとる。

      * SDL::FULLSCREEN
        
        フルスクリーンモードで動作しようとする。

      * SDL::DOUBLEBUF

        ダブルバッファリングができるようにする。
        ((<SDL::Screen#flip>))をよびだすことによってバッファの切り替え
        ができる。

      * SDL::ANYFORMAT

        SDLライブラリは要求されたbppでの設定を試みるが、それと異なっても利用
        可能なモードがあればそれを返してくる。デフォルトの動作では、要求され
        たモードが直接サポートされていない場合はエミュレーションを行う。

      flagはそのほかにもある。さらに詳しく知る必要があれば
      SDLのドキュメントを見てください。

--- SDL.checkVideoMode(w,h,bpp,flags)
      指定されたビデオモードがサポートされているかどうかを調べる。
      指定サイズのスクリーンがどんなデプスでもサポートされていない場合は
      0を返し、いくつかのデプスで指定されたサイズがサポートされていれば、
      引数で指定したものに最も近いbppの値を返す。もしこの値がビデオモード
      を設定する際に指定したものと違っている場合は、((<SDL.setVideoMode>))は
      成功するが、指定されたモードはシャドウサーフェスを使ってエミュレー
      トされることになる。
      
      SDL.checkVideoModeに対する引数は、((<SDL.setVideoMode>))で使用するもの
      と同じである。

--- SDL.listModes(flags)
      利用可能な解像度を返す。
      利用可能な解像度が存在しないときはnil返す。またあらゆる解像度が使用可
      能なときはtrueを返す。利用可能な解像度が1個以上あるときはその解像度を
      の横方向、縦方向の解像度の値をおさめた配列が1個以上入っている配列を返す。
      
      引数のflagは、((<SDL.setVideoMode>))で使用するものと同じである。

--- SDL.setGamma(redgamma,greengamma,bluegamma)
      ガンマを設定します。
      ガンマはスクリーン上での色の明るさやコントラストを調節します。
      r,g,bそれぞれのガンマ値は1.0で無調整と同等になります。

--- SDL.autoLock
      SGEが必要

      サーフィスのロックが必要なとき自動的にロックしてくれるかどうか
      を返す。デフォルトはtrue。

      さらに詳しいことを知りたければ((<Surface::lock>))を見てください。

--- SDL.autoLock=(autolocking)
      SGEが必要
      サーフィスのロックが必要なとき自動的にロックしてくれるように設定する。
      
--- SDL.videoInfo
      Videoの情報をVideoInfoのインスタンスで返す。その内容は以下の通り。
      真偽値を表すものはtrue,falseが入っている。
      詳しい意味はSDLのドキュメントを参照してください。
        SDL::VideoInfo#hw_available
        SDL::VideoInfo#wm_available
        SDL::VideoInfo#blit_hw
        SDL::VideoInfo#blit_hw_CC
        SDL::VideoInfo#blit_hw_A
        SDL::VideoInfo#blit_sw
        SDL::VideoInfo#blit_sw_CC
        SDL::VideoInfo#blit_sw_A
        SDL::VideoInfo#blit_fill
        SDL::VideoInfo#video_mem
        SDL::VideoInfo#bpp
	
--- SDL.blitSurface(src,srcX,srcY,srcW,srcH,dst,dstX,dstY)
      srcで指定されたSurfaceからdstで指定されたSurfaceへの高速なblit
      を行う。

      srcX,srcY,srcW,srcHにすべて0を指定した場合はsrc全体をblitする。

      失敗時には例外SDL::Errorが生じる。      
      
      ロックしたサーフェスに対してこれをつかってはいけない。

--- SDL.blitSurface2(src,srcRect,dst,dstRect)
      blitSurface2(src,[0,32,32,32],dst,[100,200])
      のように使う。

      srcRectにnilを指定した場合はsrc全体をblitする。

--- SDL.rotateXYScaled(src,dst,x,y,angle,xscale,yscale)
      SGEが必要

      SGEのsge_rotate_xyscaled関数とはsrcとdstの順序が入れ替わっていることに
      注意するように。下の2つの関数も同様である。
      これは、blitSurfaceに合わせたためである。
      また、この仕様は変更する可能性がある。

      また、ColorKeyは無視される。

--- SDL.rotateScaled(src,dst,x,y,angle,scale)
      ((<SDL.rotateXYScaled>))と同様、ただしxscaleとyscaleがともにscaleであると
      する。

--- SDL.rotate(src,dst,x,y,angle)
      ((<SDL.rotateXYScaled>))と同様、ただしxscaleとyscaleがともに1であると
      する。

--- SDL.rotateScaledBlit(src,dst,x,y,angle,scale)
      SGEが必要
      ColorKeyは有効となる。
      ((<SDL.rotateBlit>))も同様である。

--- SDL.rotateBlit(src,dst,x,y,angle)
      ((<SDL.rotateScaledBlit>))と同様、ただしscaleが1であるとする。

--- SDL.transform(src,dst,angle,xscale,yscale,px,py,qx,qy,flags)
      SGEが必要
      回転縮小拡大を描画する。
      src全体をpx,pyを中心にangle度回転、X方向にxscale倍、Y方向にyscale倍して
      px、pyがdstのqx、qyに一致するように描画する。
      flagの意味は以下の通り。これらのORをとってもよい。

      * 0

        普通に回転させる

      * SDL::TRANSFORM_SAFE

        srcとdstのフォーマットが違ってもうまくいくようにする。多少遅い。

      * SDL::TRANSFORM_AA

        通常より遅いがみためは良くなる。

      * SDL::TRANSFORM_TMAP

        テクスチャーマッピングを使用する。ほんの少しはやいがみためが少し
        悪くなる。px、py、flagsは無視される。

=== SDL::Surface

画像を保持するクラスです。

==== スーパークラス

Object

==== クラスメソッド

--- SDL::Surface.new(flag,w,h,format)
      新しい((<SDL::Surface>))のインスタンスを生成する。
      ((<SDL.setVideoMode>))を呼びだした後でしか使用してはならない。

      formatとしては((<SDL::Surface>))のインスタンスを与え、指定した
      サーフィスと同じbppのサーフィスを生成する。

      flagには以下のフラグのORを取ったものを与えることができる。

      * SDL::SWSURFACE

        システムメモリ内にサーフィスをとる。

      * SDL::HWSURFACE

        ビデオメモリ内にサーフィスを取ろうとする。

      * SDL::SRCCOLORKEY

        システムメモリかビデオメモリのうちハードウェアによる透明色機能を
        利用できるほうにサーフィスを取ろうとする。

      * SDL::SRCALPHA      

        システムメモリかビデオメモリのうちハードウェアによるアルファを利用
        できるほうにサーフィスを取ろうとする。
        
--- SDL::Surface.loadBMP(filename)
      指定されたファイル名のWindows BMP形式のファイルから((<SDL::Surface>))
      のインスタンスを作成する。
      ファイルが開けない等のエラーが生じた場合は((<SDL::Error>))例外が生じる。

--- SDL::Surface.load(filename)
      SDL_imageが必要

      指定されたファイル名の画像ファイルから((<SDL::Surface>))
      のインスタンスを作成する。
      BMP,PPX,XPM,PCX,GIF,JPEG,PNG,TGAなどをロードできる。

==== メソッド

--- SDL::Surface#displayFormat
      selfをビデオフレームバッファのピクセルフォーマットと色に合わせた
      新しいサーフェスにコピーして返す。
      新しいsurfaceを使うことによってscreenへの高速なblitができる。
      
      カラーキーとα値に対するblitのアクセラレーションを利用したいのならば、
      この関数の呼び出しに先立ってカラーキーとα値の設定を行っておく
      必要がある。

--- SDL::Surface#setColorKey(flag,key)
      blit可能なサーフェスに対してカラーキー(透明なピクセル)を設定する。

      flagにSDL::SRCCOLORKEYを指定すると、そのピクセルはblitでsrcになる
      ほうの透明ピクセルを示していることになる。
      通常はこのフラグを使う。
      
      flagにSDL::RLEACCELとORをとって指定すると、RLEを使って高速なblitを
      しようとする。

      flagが0の場合は、color keyをクリアする。

--- SDL::Surface#fillRect(x,y,w,h,color)
      指定された長方形の領域をcolorでぬりつぶす。

--- SDL::Surface#setClipRect(x,y,w,h)
      クリッピングをする長方形を指定する。
      ((<SDL.blitSurface>)),((<SDL::Surface#put>))等でこのインスタンスに
      描画しようとしたとき、これで指定した長方形内部のみで描画される。
      
--- SDL::Surface#setAlpha(flag,alpha)
      アルファの設定をする。
      flagにSDL::SRCALPHAを設定することでアルファが有効になる。
      SDL::RLEACCELとORをとって指定するとRLEによる高速化が有効になる。
      alphaはアルファ値を意味し、0で透明、255で通常と同等となる。

      flagに0を指定すればアルファは無効となる。

--- SDL::Surface#h
      selfの高さを返す。

--- SDL::Surface#w
      selfの横幅を返す。

--- SDL::Surface#format
      selfのpixel formatを返す。
      詳しくは((<SDL::PixelFormat>))を参照。

--- SDL::Surface#put(image,x,y)
      selfの位置(x,y)にimageを描画する。
      ((<SDL.blitSurface>))で実装されている。

--- SDL::Surface#lock
      getPixel,setPixelやdrawLine等のメソッドを使って描画する
      ための準備をする。

      ((<SDL::Surface#mustLock?>))がtrueを返したインスタンスのみこの操作
      が必要となる。

      ((<SDL::Surface#unlock>))を呼びだすまで、他ライブラリの呼びだしや
      OSに対する操作をしてはならない。

      ((<SDL.autoLock>))がtrueを返すときはライブラリが自動的にこの操作を
      してくれるのでこのメソッドを呼びだす必要はない。

--- SDL::Surface#unlock
      ((<SDL::Surface#lock>))でロックしたのを解除する。

--- SDL::Surface#mustLock?
      ((<SDL::Surface#lock>))を呼びだす必要があるときはtrueを、
      ないときはfalseを返す。

--- SDL::Surface#getPixel(x,y) 
--- SDL::Surface#[](x,y)
      SGEが必要 ロックが必要

      x,yの位置のピクセルの値を返す。

--- SDL::Surface#putPixel(x,y,pixel)
--- SDL::Surface#[]=(x,y,pixel)
      SGEが必要 ロックが必要

      x,yの位置のピクセルの値をpixelにする。
      つまり、x,yの位置にpixelの色の点を打つ。

--- SDL::Surface#drawLine(x1,y1,x2,y2,color)
      SGEが必要 ロックが必要

      色がcolorの線を(x1,y1)から(x2,y2)まで描く。

--- SDL::Surface#drawRect(x,y,w,h,color)
      SGEが必要 ロックが必要

      色がcolorの長方形を描く。中はぬりつぶさない。

--- SDL::Surface#drawCircle(x,y,r,color)
      SGEが必要 ロックが必要

      色がcolorの円を描く。中はぬりつぶさない。

--- SDL::Surface#drawFilledCircle(x,y,r,color)
      SGEが必要 ロックが必要

      色がcolorの円を描き、中をぬりつぶす。

--- SDL::Surface#drawEllispe(x,y,rx,ry,color)
      SGEが必要 ロックが必要

      色がcolorの楕円を描く。中はぬりつぶさない。

--- SDL::Surface#drawFilledEllispe(x,y,rx,ry,color)
      SGEが必要 ロックが必要

      色がcolorの楕円を描く。中をぬりつぶす。。

--- SDL::Surface#rotateScaledSurface(angle,scale,bgcolor)
      SGEが必要 

      これは、selfをangle度傾け、scale倍した画像を持つSurfaceのインスタンスを
      生成するメソッドである。回転によって生じる隙間はbgcolorで埋められる。

--- SDL::Surface#rotateSurface(angle,bgcolor)
      ((<SDL::Surface#rotateScaledSurface>))と同様。
      ただしscaleは1としたことになる。

--- SDL::Surface#transformSurface(bgcolor,angle,xscale,yscale,flags)
      selfをangle度回転し、X方向にxscale倍、Y方向にyscale倍して、生じた
      すきまをbgcolorで塗りつぶした画像を持つDSL::Surfaceのインスタンス
      を生成するメソッド。
      flagは((<SDL.transform>))と同じ。

--- SDL::Surface#mapRGB(r,g,b)
      selfのサーフィスのフォーマット(bppなど)に従ってr,g,bによって
      あらわされる色に対応したpixelの値を返す。

--- SDL::Surface#mapRGBA(r,g,b,a)
      ((<SDL::Surface#mapRGB>))と同様。ただしアルファ値も含めることができる。

--- SDL::Surface#getRGB(pixel)
      ((<SDL::Surface#mapRGB>))と逆の変換をする。返り値は [r,g,b]
      という内容の配列を返す。

--- SDL::Surface#getRGBA(pixel)
      ((<SDL::Surface#getRGB>))と同様。ただし返り値にアルファも含んでいる。
      返り値は[r,g,b,a]という内容の配列である。

--- SDL::Surface#bpp
      サーフィスのフォーマットの1ピクセルあたりのビット数を返す。

--- SDL::Surface#colorkey
      サーフィスに設定されたカラーキーの値を返す。

--- SDL::Surface#alpha
      サーフィスに設定されたアルファ値を返す。

--- SDL::Surface#setPalette(flag,colors,firstcolor)
      8bppのサーフェスにパレットを設定する。
      
      ((<SDL.setVideoMode>))でflagにSDL::HWPALETTE、bppに8を指定して得た
      スクリーンサーフィスには、2つのパレットがある。一方は論理パレットで
      ((<SDL.blitSurface>))などで画像を転送するときに用いられる。
      他方は物理パレットで、実際に画面に表示されるときの色を決定する。

      flagに SDL::LOGPALを指定すると論理パレットを、SDL::PHYSPALを指定すると
      物理パレットを変更する。両方のORを取ると、両方変更する。

      実際のパレットの値の設定の仕方は以下の通りである。
      0から255のうちXからYまでを変更したいとする。
      まずcolorsにはY-X+1の要素を持つ配列を指定する。
      その各々の要素は3つの要素を持った配列を入れる。
      それにはr,g,bの値を入れ色を指定する。そしてfirstcolorにはXを指定する。
      
      パレットの変更が成功したときはtrue、失敗したときはfalseを返す。

--- SDL::Surface#setColors(colors,firstcolor)
      ((<SDL::Surface#setColors>))のflagにSDL::LOGPAL|SDL::PHYSPALを指定
      するのと同じ。

--- SDL::Surface#getPalette
      パレットを配列で返す。その内容は以下の様になっている。
        [ [r0,g0,b0],[r1,g1,b1], ... ,[r255,g255,b255] ]
      サーフィスがパレットを持っていないときはnilを返す。

=== SDL::Screen

ここにかきこまれた画像が画面に表示される。
このクラスはただ一つしか生成できないようになっていて、
((<SDL.setVideoMode>))によってのみ生成される。
実際には、このようなクラスは存在せず、((<SDL::Surface>))に以下の特異
メソッドを追加したオブジェクトである。

==== スーパークラス

((<SDL::Surface>))

==== クラスメソッド


==== メソッド

--- SDL::Screen#updateRect(x,y,w,h)
    この関数を呼び出すと、与えられた画面上の指定された長方形領域のリストが
    確実に更新される。
    x、y、w、hがすべて0の場合、全画面を更新する。画面がロックされてい
    るときにはこの関数を呼び出さないようにする。

--- SDL::Screen#flip
      ダブルバッファをサポートしているハードウェア上では、この関数は
      バッファの交換を要求して返る。ハードウェアによって次の垂直帰線期間まで
      待ってから、次のビデオサーフェスへのblitやロック操作の呼び出しが返る前
      にビデオバッファの交換が行われる。ダブルバッファをサポートしない
      ハードウェア上では、この呼び出しは((<SDL::Screen#updateRect>))(0,0,0,0)
      としているのと同じである。このメソッドを使用してハードウェアによる画面の
      交換を行うためには、((<SDL.setVideoMode>))の引数でDOUBLEBUFフラグを設
      定しておくことが必要である。

=== SDL::PixelFormat

このクラスの機能はすべて((<SDL::Surface>))にうつされた。そちらを使用したほうが
よい。このクラスは互換性のために残してある。

SDL_PixelFormatのラッパークラス。
((<SDL::Surface#format>))で取得できる。

SDLでは、16bit colorのsurfaceと32bitのsurfaceというように複数の画像の形式が
共存できる。よってその画像を操作するため、その形式に関する情報が
必要となる。PixelFormatのインスタンスが表しているものはそれである。

通常色を表すためにはr,g,bの値が使われるが、それをsurfaceの内部形式に従った
ひとつの値にするためにこれを用いる。逆の変換もできる。

このライブラリ内ではこれによって指定された値をもちいて色を指定する。

また、配列を用いて色を表すこともできる。これは r,g,b の値を
[r,g,b]で表すものである。

==== スーパークラス

Object

==== クラスメソッド

なし

==== メソッド

--- SDL::PixelFormat#mapRGB(r,g,b)
      selfのフォーマットに従ってr,g,bによってあらわされる色に対応した
      pixelの値を返す。

--- SDL::PixelFormat#mapRGBA(r,g,b,a)
      ((<SDL::PixelFormat#mapRGB>))と同様。ただしアルファ値も含めることができる。

--- SDL::PixelFormat#getRGB(pixel)
      ((<SDL::PixelFormat#mapRGB>))と逆の変換をする。返り値は [r,g,b]
      という内容の配列を返す。

--- SDL::PixelFormat#getRGBA(pixel)
      ((<SDL::PixelFormat#getRGB>))と同様。ただし返り値にアルファも含んでいる。
      返り値は[r,g,b,a]という内容の配列である。

--- SDL::PixelFormat#bpp
      フォーマットの1ピクセルあたりのビット数を返す。

--- SDL::PixelFormat#colorkey
      Not documented yet

--- SDL::PixelFormat#alpha
      Not documented yet

== Event関連

=== SDL::Event

イベントをとりあつかうクラス

==== スーパークラス

Object

==== クラスメソッド

--- SDL::Event.new

==== メソッド

--- SDL::Event#poll
      現在イベントキュー内で処理待ちしているイベントがないかをしらべ、
      ひとつでもあれば1を、ひとつもなければ0を返す。
      また、処理待ちをしているイベントがあれば、その情報をselfに格納する。

--- SDL::Event#wait
      次のイベントが来るまで待ち、来れば1を、またイベントを待つ間にエラー
      が生じた場合は0を返す。そしてその情報をselfに格納する。

--- SDL::Event#type
      格納されているイベントがいかなる種類のものであるかを返す。
      その種類は以下の定数で示される。
      
        SDL::Event::ACTIVEEVENT 
        SDL::Event::KEYDOWN
        SDL::Event::KEYUP
        SDL::Event::MOUSEMOTION
        SDL::Event::MOUSEBUTTONDOWN
        SDL::Event::MOUSEBUTTONUP
        SDL::Event::JOYAXISMOTION
        SDL::Event::JOYBALLMOTION
        SDL::Event::JOYHATMOTION
        SDL::Event::JOYBUTTONDOWN
        SDL::Event::JOYBUTTONUP
        SDL::Event::QUIT
        SDL::Event::SYSWMEVENT
        SDL::Event::VIDEORESIZE

--- SDL::Event#info
      イベントの情報を配列で返す。
      このメソッドは、これ以下のメソッドをすべて代用できる。

--- SDL::Event#keyPress?
      キーイベントでキーが押し下げられていればtrueを、いなければfalseを返す。

--- SDL::Event#keySym
      キーイベントで押し下げ/上げられたキーをを返す。

--- SDL::Event#keyMod
      キーイベントでの修飾キー(SHIFT,CTRLなど)の状態を返す。

--- SDL::Event#gain?
      ACTIVEEVENTイベントでウィンドウがフォーカスを得たならtrueを、
      失なったならばfalseを返す。

--- SDL::Event#appState
      ACTIVEEVENTイベントでのイベントの種類を返す。
      その内容は以下のいずれか。
        SDL::Event::APPMOUSEFOCUS
        SDL::Event::APPINPUTFOCUS
        SDL::Event::APPACTIVE

--- SDL::Event#mouseX
      マウスイベントでのマウスカーソルのX座標を返す。

--- SDL::Event#mouseY
      マウスイベントでのマウスカーソルのY座標を返す。      

--- SDL::Event#mouseXrel
      マウスイベントでのマウスカーソルのX座標の変化量を返す。

--- SDL::Event#mouseYrel
      マウスイベントでのマウスカーソルのX座標の変化量を返す。

--- SDL::Event#mouseButton
      マウスイベントでどのボタンのイベントであるかをかえす。
      それは以下の定数でしめされる。

        SDL::Mouse::BUTTON_LEFT  左ボタン
        SDL::Mouse::BUTTON_MIDDLE  中ボタン
        SDL::Mouse::BUTTON_RIGHT 右ボタン

--- SDL::Event#mousePress?
      MOUSEBUTTONDOWN,MOUSEBUTTONUPイベントにおいて、マウスボタンが
      押されたならtrue、離されたならfalseを返す。

=== SDL::Key

キーボードのキーに対応する定数を定義しているモジュール
キーボードのキーの状態を知るための関数もこの中にある。

==== モジュール関数

--- SDL::Key.scan
      キーボードの状態をスキャンします

--- SDL::Key.press?(key)
      これを呼び出す前にscanを呼びだしてください。
      scan で得た状態を得ます。trueで押している、falseで離しているです。

--- SDL::Key.modState
      修飾キー(CTRL,ATL,など)の状態を返す。
      返り値は以下の定数でORをとったもので表される。
        SDL::Key::MOD_NONE
        SDL::Key::MOD_LSHIFT
        SDL::Key::MOD_RSHIFT
        SDL::Key::MOD_LCTRL
        SDL::Key::MOD_RCTRL
        SDL::Key::MOD_LALT
        SDL::Key::MOD_RALT
        SDL::Key::MOD_LMETA
        SDL::Key::MOD_RMETA
        SDL::Key::MOD_NUM
        SDL::Key::MOD_CAPS
        SDL::Key::MOD_MODE
        SDL::Key::MOD_RESERVED
        SDL::Key::MOD_CTRL = SDL::Key::MOD_LCTRL|SDL::Key::MOD_RCTRL
        SDL::Key::MOD_SHIFT = SDL::Key::MOD_LSHIFT|SDL::Key::MOD_RSHIFT
        SDL::Key::MOD_ALT = SDL::Key::MOD_LALT|SDL::Key::MOD_RALT
        SDL::Key::MOD_META = SDL::Key::MOD_LMETA|SDL::Key::MOD_RMETA

--- SDL::Key.enableKeyRepeat(delay,interval)
      キーリピートの設定を変える。
      

--- SDL::Key.disableKeyRepeat
      キーリピートを無効にする。
    
== SDL::Mouse

マウス関連の定数と関数を定義しているモジュール

=== モジュール関数

--- SDL::Mouse.state
      マウスの状態を配列の形で返します。
      配列の内容は
        [ x , y , pressLButton? , pressMButton? , pressRButton? ]
      となっています。

--- SDL::Mouse.warp(x,y)
      マウスカーソルの位置を設定する（マウス移動のイベントを生成する)。

--- SDL::Mouse.show
      マウスカーソルを表示する。

--- SDL::Mouse.hide
      マウスカーソルを消す。

--- SDL::Mouse.setCursor(bitmap,white,black,transparent,inverted,hot_x=0,hot_y=0)
      マウスカーソルを変える。
      bitmapとして((<SDL::Surface>))のインスタンスを与える。
      カーソルの色は白黒で生成される。
      white,black,transparent,invertedはそのサーフィス内で、どの
      ピクセルがカーソルの白、黒、透明、反転にあたるかを示す。

== audio関連

=== SDL::Mixer

音をだしたりするのに使うモジュール
ボリュームは0から128が有効である。
このモジュールの機能を使うためには、SDL_mixerライブラリが必要である。
このモジュール内の機能を使うためには、((<SDL.init>))でSDL::INIT_AUDIOを
有効にしなければならない。

==== モジュール関数

--- SDL::Mixer.open(frequency=Mixer::DEFAULT_FREQUENCY,format=Mixer::DEFAULT_FORMAT,cannels=Mixer::DEFAULT_CHANNELS,chunksize=4096)
      このモジュールの機能の初期化関数。
      frequencyは周波数、formatはサウンドの形式、
      channelsは1でモノラル、2でステレオ
      chunksizeはバッファの大きさ、をそれぞれ指定する。
      chunksizeは2の階乗を使うようにする。
      ここでいうchannelsとplayChannelなどでのchannelは別物である。

--- SDL::Mixer.spec
      初期化したオーディオの性能を配列で返す。
      その内容は、
        [ rate,format,channels ]

--- SDL::Mixer.playChannel(channel,wave,loop)
      指定したchannelでwaveを演奏する。
      channelに-1を指定すると、あいているchannelが適当にえらばれる。
      loops指定した回数繰り返す。
      loopsが-1のときは際限なくくりかえす。
      loopsが0のときは一度のみ演奏する。

      どのチャンネルを演奏に利用したかを返す。

--- SDL::Mixer.play?(channel)
      指定したchannelが現在演奏していればtrueを、していなければ
      falseを返す。

--- SDL::Mixer.setVolume(channel,volume)
      指定したchannelのボリュームを設定する。
      channel=-1を指定するとすべてのchannelに対しボリュームを指定する。

      volume=-1とすると、現在のボリュームが返る。

--- SDL::Mixer.halt(channel)
      指定したchannelの演奏を止める。

--- SDL::Mixer.pause(chennel)
      指定したchannelの演奏を一時停止する。

--- SDL::Mixer.resume(channel)
      指定した一時停止しているchannelの演奏を再開する。

--- SDL::Mixer.pause?(channel)
      指定したchannelが一時停止していればtrue、していなければfalseを
      返す。

--- SDL::Mixer.playMusic(music,loops)
      musicで指定した音楽を演奏する。
      loopsは((<SDL::Mixer.playChannel>))と同じ。

--- SDL::Mixer.fadeInMusic(music,loops,ms)
      musicで指定した音楽をフェードインして演奏する。
      loopsは((<SDL::Mixer.playChannel>))と同じ。
      フェードインはmsは指定したミリ秒だけかける

--- SDL::Mixer.setVolumeMusic(volume)
      音楽のボリュームを指定する。

--- SDL::Mixer.haltMusic
      音楽を止める。

--- SDL::Mixer.fadeOutMusic(ms)
      音楽を指定したミリ秒かけてフェードアウトする。

--- SDL::Mixer.pauseMusic
      音楽を一時停止する。
      
--- SDL::Mixer.resumeMusic
      一時停止している音楽の再生を再開する。

--- SDL::Mixer.rewindMusic
      音楽の再生位置を一番最初にする。

--- SDL::Mixer.pauseMusic?
      音楽が一時停止していればtrue、いなければfalseを返す。

--- SDL::Mixer.playMusic?
      音楽が演奏されていればtrue、していなければfalseを返す。

=== SDL::Mixer::Wave

Waveをあらわすクラス

==== スーパークラス

Object

==== クラスメソッド

--- SDL::Mixer::Wave.load(filename)
      waveファイルをロードし、それに対応するSDL::Mixer::Waveクラスの
      インスタンスを返す。

==== メソッド

--- SDL::Mixer::Wave#setVolume(volume)
      selfのボリュームを返す。

=== SDL::Mixer::Music

音楽(.mod .s3m .it .xm .mid .mp3)を表す。
ただしmidiファイルやMP3を演奏するためにはそのための設定が必要。

==== スーパークラス

Object

==== クラスメソッド

--- SDL::Mixer::Music.load(filename)
      音楽(.mod .s3m .it .xm .mid .mp3)をファイルからロードし、
      そのデータに対応するMixer::Musicクラスのインスタンスを返す。

== Window Manager 関連

=== SDL::WM

Window関連の処理をまとめたモジュール

==== モジュール関数

--- SDL::WM.caption
      captionの文字列を配列で返してくる。
      内容は、
        [ ウィンドウのタイトル , アイコンのタイトル ]

--- SDL::WM.setCaption(title,icon)
      上記の内容を変更する。

--- SDL::WM.icon=(iconImage)
      ウィンドウのアイコンの絵を指定する。
      setVideoModeの呼びだし前に呼びださなければならない。
      
--- SDL::WM.iconify
      ウィンドウのアイコン化、最小化をする。
      成功すれば、SDL::Event::APPACTIVEイベントが生じる。

== CDROM関係

=== SDL::CD

CDROMドライブを表すクラス

CD#numTrack等の情報はCD#statusを呼びだすことによって更新される。

ここではCDの位置、演奏時間の長さを表すためにフレームという単位を用いる。
1frame=2Kであり、
通常の音楽CDにおいて75フレーム=1秒である。

=== スーパークラス

Object

=== クラスメソッド

--- SDL::CD.numDrive
      いくつのCDROMドライブが使用できるかを返す。

--- SDL::CD.name(drive)
      指定したドライブの名前を文字列で返す。
      ドライブの指定は0からCD.numDrive-1の整数で指定する。

--- SDL::CD.open(drive)
      指定したドライブを開く。
      成功すればCDのインスタンスが返る。
      ドライブの指定は0からCD.numDrive-1の整数で指定する。
      0ドライブがデフォルトのドライブである。

=== メソッド

--- SDL::CD#status
      currentTrack,currentFrame,numTracks,trackType,trackLenght
      の情報を更新する。
      現在のCDの状態を返す。その内容は以下のとおり。
        TRAYEMPTY
        STOPPED
        PLAYING
        PAUSED
        ERROR

--- SDL::CD#play(start,length)
      CDをstartフレームからlengthフレームの間演奏する。

--- SDL::CD#playTrack(start_track,start_frame,ntracks,nframes)
      CDをstart_trackのstart_frameから、ntracks先のトラックのnframeのところ
      まで演奏する。
      なお、このライブラリ内ではトラックのインデックスは0からはじまる。
      このメソッドは((<SDL::CD#status>))を呼びだしたあとに呼びださな
      ければならない。

        例
        cd.playTrack(0,0,1,0) # 最初のトラックを演奏する。
        cd.playTrack(1,0,0,SDL::CD::FPS*15) # 2トラック目を最初から15秒演奏する。

--- SDL::CD#pause
      CDの演奏を一時停止する。

--- SDL::CD#resume
      CDの演奏を再開する。

--- SDL::CD#stop
      CDの演奏を止める。

--- SDL::CD#eject
      CDをイジェクトする。

--- SDL::CD#numTracks
      CDのトラック数を返す。

--- SDL::CD#currentTrack
      現在演奏しているトラックを返す。

--- SDL::CD#currentFrame
      現在演奏している位置をフレーム数で返す。
      このあたいは現在のトラックの最初からの値である。

--- SDL::CD#trackType(track)
      指定したトラックがなんであるかを返す。その値は以下のとおり。
        SDL::CD::AUDIO_TRACK
        SDL::CD::DATA_TRACK

--- SDL::CD#trackLength(track)
      指定したトラックの長さをフレーム数で返す。

== ジョイスティック関連

=== SDL::Joystick

一つのジョイスティックを表すクラス

=== スーパークラス

Object

=== クラスメソッド

--- SDL::Joystick.pall
      イベントのpollingに合わせて、((<SDL::Joystick.updateAll>))を呼び
      だすかどうかを得る。
      
--- SDL::Joystick.pall=(polling)
      イベントのpollingに合わせて、((<SDL::Joystick.updateAll>))を呼びだす
      かどうかを設定する。

--- SDL::Joystick.num
      利用可能なジョイスティックの数を返す。

--- SDL::Joystick.name(index)
      指定したジョイスティックの名前を文字列で返す。
      0からJoystick.num-1の整数で指定する。

--- SDL::Joystick.open(index)
      指定したジョイスティックを開く。
      成功するとJoystickのインスタンスが返ってくる。

--- SDL::Joystick.open?(index)
      指定したジョイスティックがすでに開かれているかを返す。

--- SDL::Joystick.updateAll
      全てのジョイスティックの情報を更新する。
      Joystick#button/ball/hat/axisの情報はこれで更新される。

=== メソッド

--- SDL::Joystick#index
      ジョイスティックに対応する整数を返す。

--- SDL::Joystick#numAxes
      いわゆるアナログ入力装置の数を返す。
      ただし、2軸のアナログティックは2個の装置があると数えられる。

--- SDL::Joystick#numBalls
      トラックボールの数を返す。

--- SDL::Joystick#numButtons
      ボタンの数を返す。

--- SDL::Joystick#axis(axis_index)
      アナログ入力装置の入力状態を返す。
      その値は-32768から32768である。
      通常0にはx軸、1にはy軸があてられる。

--- SDL::Joystick#hat(hat_index)
      いわゆる十字キーの入力状態を返す。その内容は以下のとおり。
        SDL::Joystick::HAT_CENTERED
        SDL::Joystick::HAT_UP
        SDL::Joystick::HAT_RIGHT
        SDL::Joystick::HAT_DOWN
        SDL::Joystick::HAT_LEFT
        SDL::Joystick::HAT_RIGHTUP
        SDL::Joystick::HAT_RIGHTDOWN
        SDL::Joystick::HAT_LEFTUP
        SDL::Joystick::HAT_LEFTDOWN

--- SDL::Joystick#button(button_index)
      ボタンの押下情報を返す。
      押していればtrue、いなければfalseを返す。

--- SDL::Joystick#ball(ball_index)
      トラックボールの入力情報を返す。
      その内容は最後にこのメソッドを呼び出してからX軸、Y軸にどれだけ
      動かしたかを配列 [dx,dy] で返す。

== フォント関係

=== SDL::TTF

ＴｒｕｅＴｙｐｅフォントを表すクラス

==== スーパークラス

Object

==== クラスメソッド

--- SDL::TTF.init
      TrueTypeFontを使うためには最初にこの関数を呼ばなければならない。

--- SDL::TTF.open(filename,size)
      TrueTypeFontをファイルから読み出し、そのサイズをsizeで指定する。
      
==== メソッド

--- SDL::TTF#style
      フォントのスタイルを返す。

--- SDL::TTF#style=(style)
      フォントのスタイルを設定する。指定できるのは以下の定数でORをとったもの。
        SDL::TTF::STYLE_NORMAL
        SDL::TTF::STYLE_BOLD
        SDL::TTF::STYLE_ITALIC
        SDL::TTF::STYLE_UNDERLINE

--- SDL::TTF#textSize(text)
      textを描画したときの縦、横の必要な大きさを配列で返す

--- SDL::TTF#drawSolidUTF8(dest,text,x,y,r,g,b)
      selfのフォント設定でdest(Surfaceのインスタンス)にString textを
      destの位置(x,y)の所に書きこむ。色はr,g,bで決められる。
      透明色(ColorKey)は有効である。textはUTF-8を使う。

--- SDL::TTF#drawBlendedUTF8(dest,text,x,y,r,g,b)
      drawSolidUTF8と同様。drawSolidUTF8よりも高品質な描画ができる。

== 時刻処理

=== モジュール関数

--- SDL.getTicks
      ((<SDL.init>))が呼ばれてからの時間をミリ秒で返す。
      49日ほどスクリプトを走らせつづけると0にもどるのに注意。

--- SDL.delay(ms)
      ミリ秒で指定された時間だけ待つ。
      OSのスケジューリングのため指定した時間よりも長く待つ可能性がある。

=end
