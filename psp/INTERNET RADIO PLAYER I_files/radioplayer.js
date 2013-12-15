// ==========================================================================
// Copyright (C) 2007-2010 Sony Computer Entertainment Inc.
// All Rights Reserved. http://radio.psp.dl.playstation.net/psp/radio/player/PLAYER_I/
// ==========================================================================

var isPSPRadio = false;
var coreVersion = "";
var bAacpSupport = false;
var bSleepSupport = false;

// --------------------------------------------------------------------------
// PSP 拡張プラグインオブジェクトの生成
// --------------------------------------------------------------------------

if ( navigator.mimeTypes
     && navigator.mimeTypes ["application/x-psp-extplugin"] ) {
    var plugin
		= navigator.mimeTypes ["application/x-psp-extplugin"].enabledPlugin;
    if ( plugin ) {
        document.write
			("<object name=psp type=\"application/x-psp-extplugin\" >"
			 +"</object>\n");
    }
}
else psp = null;

window.onload = onLoadProc;
window.onunload = onUnLoadProc;

// --------------------------------------------------------------------------
// 文字列定数など
// --------------------------------------------------------------------------

var userAgentForPlayStream = "PSP-InternetRadioPlayer/1.00";
var userAgentForHttpGet = "PSP-InternetRadioPlayer/1.00";
var userAgentForGetPls = "PSP-InternetRadioPlayer/1.00";

var playerName = "PLAYER_I";
var playerTopPageUrl = "http://radio.psp.dl.playstation.net/psp/radio/player/"
	+ playerName + "/index.html";

// --------------------------------------------------------------------------
// グローバル変数など
// --------------------------------------------------------------------------

var timerID_for_contentMetaInfo=(0);
var timerID_for_streamUrl=(0);
var timerID_for_analogStick=(0);
var timerID_for_httpGetProc=(0);
var timerID_for_streamStatusCheckProc=(0);
var timerID_for_magicWindow=(0);
var timerID_for_startButtonStatusPolling=(0);
var timerID_for_sleepCountDownProc=(0);
var timerID_for_hideSleepMessageProc=(0);
var timerID_for_dayTick=(0);

// 状態変数
var sw_shift_status=(1);
var sw_sin_status=(1);
var sw_cw_status=(1);
var sw_noise_status=(1);

// スリープ処理用変数
var sleepCountDown = 0;
var bSleepTimerWasActivated = false;
var prevSleepCountDownProcTimestamp = 0;
var bErrorDetectorForSleepCountDownProc = false;

// クッキーに記録されていたジャンル選択に従ってジャンルに属する放送局リストを
// 取得したときに自動的にリスト先頭局に選局しないようにするため。
// つまり、クッキー情報に基づいて選局した状態を維持するため。
var bIgnoreAutoTuneInToTopStationWhenGotStationList = true;

// ミュート処理用フラグ
var bMute = false;

var bCollectingStations = false; // ジャンルが切り替えられたとき、放送局をスキャン中に true

// コンテントメタインフォ表示関連
var messageState = 0;

var lastTimeStationInfo = new Object ();
lastTimeStationInfo.genre = -1;
lastTimeStationInfo.streamUrl = "";
lastTimeStationInfo.stationName = "";
lastTimeStationInfo.comment = "";
lastTimeStationInfo.refPage = "";
lastTimeStationInfo.br = "";
lastTimeStationInfo.sr = "";
lastTimeStationInfo.aacp = false;

var streamTitle="";
var contentMetaInfo="";
var lastArtistAndSongName="";

var langID = 0;
var usageLang = "en";
var refPageIsClosed = true;
var streamStatusCheckProcWorkState = 0;
var bBusy = false;
var bConnectError = false;
var bInAnalizingStationListString = false;
var bForcedExitFlag = false;
var bGoingToSleep = false;

var sinWaveTension = 16; // デフォルトは 15.984375

var maxNumStation = 30;

var startDate = "";

// --------------------------------------------------------------------------
// メッセージ
// --------------------------------------------------------------------------

messageList_en = new Array (
	"SCANNING...",
	"CONNECTING...",
	"CONNECTION ERROR",
	"There are no stations of the selected genre.",
	"MUTING...",
	"If you press the START button, muting will be cleared.",
	"Other Stations",
	"To use the sleep timer, a system software update for the PSP™ system is required.\nFor update methods, refer to the instructions for the PSP™ system.",
	"The system is going to switch to sleep mode soon."
);
messageList_ja = new Array (
	"スキャン中…",
	"接続中…",
	"接続エラー",
	"該当する放送局がありません",
	"消音中…",
	"“START” ボタンを押す事で消音状態の解除ができます。",
	"他の放送局",
	"スリープタイマーを使用するには、本機のシステムソフトウェアのアップデートが必要です。\nアップデートの方法については本機の取扱説明書をご覧ください。",
	"まもなくスリープモードに切りかわります。"
);
messageList_de = new Array (
	"Scannt ...",
	"Verbindet ...",
	"Verbindungsfehler",
	"Für das ausgewählte Genre sind keine Sender vorhanden.",
	"Stummschaltung...",
	"Wenn Sie die START-Taste drücken, wird der Ton des Systems stummgeschaltet.",
	"Andere Sender",
	"Um den Energiespar-Timer zu verwenden, muss die Software des PSP™-Systems aktualisiert werden.",
	"Das System wird bald in den Energiesparmodus versetzt."
);
messageList_es = new Array (
	"Escaneando...",
	"Conectando...",
	"Error de conexión",
	"No hay estaciones del género seleccionado.",
	"Silenciando...",
	"Si pulsa el botón START (inicio), el audio del sistema se silenciará.",
	"Otras estaciones",
	"Para utilizar el temporizador de suspensión, se requiere una actualización del software del sistema de PSP™.",
	"El sistema está a punto de pasar al modo de suspensión."
);
messageList_fr = new Array (
	"Scan en cours...",
	"Connexion en cours...",
	"Erreur de connexion",
	"Il n'y a aucune station pour le genre sélectionné.",
	"Mise en sourdine...",
	"Si vous appuyez sur la touche START, l'audio du système sera mise en sourdine.",
	"Autres stations",
	"L'utilisation du minuteur de mise en veille nécessite une mise à jour du logiciel système de votre PSP™.",
	"Le système va bientôt passer en mode de veille."
);
messageList_it = new Array (
	"Scansione in corso...",
	"Connessione in corso...",
	"Errore di connessione",
	"Non vi sono stazioni del genere selezionato.",
	"Disattivazione audio...",
	"Premendo il tasto START, l'audio del sistema sarà disattivato.",
	"Altre stazioni",
	"Per poter utilizzare il timer della modalità di sospensione, occorre aggiornare il software del sistema PSP™.",
	"Il sistema sta per passare alla modalità di sospensione."
);
messageList_nl = new Array (
	"Bezig met scannen...",
	"Bezig met verbinden...",
	"Verbindingsfout",
	"Er zijn geen stations met het geselecteerde genre.",
	"Dempen...",
	"De audio van het systeem zal worden gedempt wanneer u op de START-toets drukt.",
	"Andere stations",
	"Voor gebruik van de stand-by timer is een software update voor het PSP™-systeem nodig.",
	"Het systeem schakelt spoedig over naar modus Stand-by."
);
messageList_pt = new Array (
	"A examinar...",
	"A ligar...",
	"Erro de ligação",
	"Não existem estações do género seleccionado.",
	"A desactivar som...",
	"Se premir o botão START, desactivará o áudio do sistema.",
	"Outras estações",
	"Para se utilizar o temporizador de descanso, é necessária uma actualização do software do sistema para o sistema PSP™.",
	"O sistema irá brevemente mudar para o modo de descanso."
);
messageList_ru = new Array (
	"Идет сканирование...",
	"Подключение к станции...",
	"Ошибка подключения",
	"Станций подобного жанра не найдено.",
	"Выключение звука...",
	"При нажатии кнопки START (ПУСК) звук аудиосистемы будет выключен.",
	"Другие станции",
	"Для использования таймера режима ожидания требуется обновление программного обеспечения системы PSP™.",
	"Скоро система перейдет в режим ожидания."
);
messageList_kr = new Array (
	"검색중...",
	"접속중...",
	"접속 에러",
	"선택한 장르의 방송국이 없습니다.",
	"음소거 중...",
	"START 버튼을 누르면  음소거 상태가 됩니다.",
	"다른 방송국",
	"슬립 타이머를 사용하시려면 본체의 시스템 소프트웨어 업데이트가 필요합니다.\n업데이트 방법에 대해서는 본 기기의 사용설명서를 참조해 주십시오.",
	"잠시 후에 슬립 모드로 전환됩니다."
);
messageList_zh_tw = new Array (
	"掃描中……",
	"連線中……",
	"連線錯誤",
	"沒有您所選擇的類型的播放電台。",
	"靜音中……",
	"按下START（開始）按鈕，即會靜音。",
	"其他播放電台",
	"若要使用睡眠定時設定，需先更新本主機的系統軟件。\n若您希望了解如何更新，請參閱本主機的使用說明書。",
	"主機即將進入睡眠模式。"
);
messageList_zh_cn = new Array (
	"正在搜索...",
	"正在连接...",
	"连接错误",
	"没有所选择类型的电台。",
	"静音中...",
	"按START（开始）键将静音。",
	"其他电台",
	"要使用休眠计时器，需进行PSP™主机的系统软件升级。\n关于升级的方法，请参阅PSP™主机的使用说明书。",
	"主机即将进入休眠模式。"
);

messageList = new Array (
	 messageList_en,
	 messageList_ja,
	 messageList_de,
	 messageList_es,
	 messageList_fr,
	 messageList_it,
	 messageList_nl,
	 messageList_pt,
	 messageList_ru,
	 messageList_kr,
	 messageList_zh_tw,
	 messageList_zh_cn
);


// --------------------------------------------------------------------------
// クッキーへの書き出し
// --------------------------------------------------------------------------

var bDoUpdateStreamUrl = false;
var selectedGenre = -1;

function writeIntoCookie () {
	bDoUpdateStreamUrl = true;
}

// --------------------------------------------------------------------------
// 初期化処理
// --------------------------------------------------------------------------

function getCookieValue ( keyword ) {
	// クッキーからの値の読み出し
	keyword += ":";
	var cookieValue;
	if ( document.cookie ) cookieValue = unescape (document.cookie) + ";";
	else {
		// 初回起動時は 80s ジャンルを選択するものとする。
		cookieValue
			= "IRP1=genre:3;streamUrl:;shiftSw:1;sinSw:1;cwSw:1;noiseSw:1;";
		bIgnoreAutoTuneInToTopStationWhenGotStationList = false;
	}
	var start, end;
	start = cookieValue.indexOf (keyword, cookieValue.indexOf ("IRP1="));
	if ( start != -1 ) {
		end = cookieValue.indexOf (";", start);
		return ( cookieValue.substring (start + keyword.length, end) );
	}
	else return ( "" );
}

/*----*/
var bInited = false;
function onLoadProc () {
	// 表示言語値の取得とラジオプレイヤ内部の言語 ID 値の設定
	{
		var lang = navigator ["language"];
		switch ( lang ) {
		case "ja":
			langID = 1;
			usageLang = "jp";
			break;
		case "de":
			langID = 2;
			usageLang = "de";
			break;
		case "es":
			langID = 3;
			usageLang = "es";
			break;
		case "fr":
			langID = 4;
			usageLang = "fr";
			break;
		case "it":
			langID = 5;
			usageLang = "it";
			break;
		case "nl":
			langID = 6;
			usageLang = "nl";
			break;
		case "pt":
			langID = 7;
			usageLang = "pt";
			break;
		case "ru":
			langID = 8;
			usageLang = "ru";
			break;
		case "ko":
			langID = 9;
			usageLang = "kr";
			break;
		case "zh-tw":
			langID = 10;
			usageLang = "ct";
			break;
		case "zh-cn":
			langID = 11;
			usageLang = "en";
			break;
		case "en":
		default:
			langID = 0;
			usageLang = "en";
			break;
		}
	}

	// 選局スイッチの横に表示する文言
	document.getElementById ("prevStation").firstChild.nodeValue
		= document.getElementById ("nextStation").firstChild.nodeValue
		= messageList [langID] [6];

	// クッキーに記録された、前回起動時の最後に再生していた放送局に関する情報の取得
	// -- ジャンル --
	lastTimeStationInfo.genre = eval (getCookieValue ("genre"));
	selectedGenre = lastTimeStationInfo.genre;
	// -- ストリーム URL --
	lastTimeStationInfo.streamUrl = getCookieValue ("streamUrl");
	// -- 放送局名 --
	lastTimeStationInfo.stationName = getCookieValue ("stationName");
	// -- コメント --
	lastTimeStationInfo.comment = getCookieValue ("comment");
	// -- 関連ページ --
	lastTimeStationInfo.refPage = getCookieValue ("refPage");
	// -- ビットレート --
	lastTimeStationInfo.br = getCookieValue ("br");
	// -- サンプルレート --
	lastTimeStationInfo.sr = getCookieValue ("sr");
	// -- フォーマット --
	lastTimeStationInfo.aacp = eval (getCookieValue ("aacp"));

	if ( psp ) {
		coreVersion = getCookieValue ("coreVersion");
		if ( coreVersion != psp.sysRadioGetPlayerCoreVersionInfo () ) {
			;
			// 古いバージョンの情報では不都合がある場合には
			// クッキーから得た情報を破棄する
			coreVersion = psp.sysRadioGetPlayerCoreVersionInfo ();
		}
		bAacpSupport = psp.sysRadioCapabilityCheck (0) ? true : false;
		bSleepSupport = psp.sysRadioCapabilityCheck (1) ? true : false;
	}

	// クッキーに記録された、トグルスイッチの状態
	sw_shift_status = eval (getCookieValue ("shiftSw"));
	sw_sin_status = eval (getCookieValue ("sinSw"));
	sw_cw_status = eval (getCookieValue ("cwSw"));
	sw_noise_status = eval (getCookieValue ("noiseSw"));

	initGenreLabel ();

	radioPlayerInit0 (lastTimeStationInfo.streamUrl);

	clearGenreListSelection ();
	prepareForGetStationList ();

	initStationMarker ();

	setScaleHand (-1);

	// イメージを予めキャッシュさせる
	imagePreLoader ();
	// トグルスイッの初期化
	initToggleSw ();

    timerID_for_contentMetaInfo
		= setTimeout ('updateContentMetaInfo ()', 2000);
    timerID_for_streamUrl
		= setTimeout ('updateStreamUrl ()', 1000);
	timerID_for_streamStatusCheckProc
		= setTimeout ('streamStatusCheckProc ()', 1500);
    timerID_for_magicWindow
		= setInterval ('updateMagicWindowProc ()', 2500);
	timerID_for_startButtonStatusPolling
		= setInterval ('startButtonPollingProc ()', 2600);
	timerID_for_dayTick
		= setInterval ('dayTickProc ()', 24*3600*1000);

	var d = new Date ();
	startDate = (1900 + d.getYear ()) + "." + (d.getMonth () + 1) + "." + d.getDate ();
}

/*----*/
function initStationMarker () {
	var i, n = maxNumStation;
	var x = 19;
	for ( i = 0; i < n; i++ ) {
		eval ("document.stationP"+i+".style.left = x;");
		x += 15;
	}
}

/*----*/
function initToggleSw () {
	if ( sw_shift_status == 0 ) {
		document.shiftTglSw.src = "images/sw_shift_small_off.png";
		document.shiftSwIndicator.src = "images/indicator_off.png";
	}
	else {
		document.shiftTglSw.src = "images/sw_shift_small_on.png";
		document.shiftSwIndicator.src = "images/indicator_on.png";
	}
	if ( sw_sin_status == 0 ) {
		document.sinTglSw.src = "images/sw_sin_small_off.png";
		document.sinSwIndicator.src = "images/indicator_off.png";
	}
	else {
		document.sinTglSw.src = "images/sw_sin_small_on.png";
		document.sinSwIndicator.src = "images/indicator_on.png";
	}
	if ( sw_cw_status == 0 ) {
		document.cwTglSw.src = "images/sw_cw_small_off.png";
		document.cwSwIndicator.src = "images/indicator_off.png";
	}
	else {
		document.cwTglSw.src = "images/sw_cw_small_on.png";
		document.cwSwIndicator.src = "images/indicator_on.png";
	}
	if ( sw_noise_status == 0 ) {
		document.noiseTglSw.src = "images/sw_noise_small_off.png";
		document.noiseSwIndicator.src = "images/indicator_off.png";
	}
	else {
		document.noiseTglSw.src = "images/sw_noise_small_on.png";
		document.noiseSwIndicator.src = "images/indicator_on.png";
	}
}

/*----*/
function setStationMarker ( numStation ) {
	if ( maxNumStation < numStation ) numStation = maxNumStation;
	var i;
	for ( i = 0; i < numStation; i++ ) {
		eval ("document.stationP"+i+".src = \"images/stationMarker_on.gif\";");
	}
	for ( ; i < maxNumStation; i++ ) {
		eval ("document.stationP"+i+".src = \"images/stationMarker_off.gif\";");
	}
}

/*----*/
function imagePreLoader () {
	if ( psp ) {
		document.leftMWindow.src = "images/magic_window0_0.png";
		document.leftMWindow.src = "images/magic_window0_1.png";
		document.leftMWindow.src = "images/magic_window0_2.png";
		document.leftMWindow.src = "images/magic_window0_3.png";

		document.centerMWindow.src = "images/magic_window1_0.png";
		document.centerMWindow.src = "images/magic_window1_1.png";
		document.centerMWindow.src = "images/magic_window1_2.png";
		document.centerMWindow.src = "images/magic_window1_3.png";

		document.rightMWindow.src = "images/magic_window2_0.png";
		document.rightMWindow.src = "images/magic_window2_1.png";
		document.rightMWindow.src = "images/magic_window2_2.png";
		document.rightMWindow.src = "images/magic_window2_3.png";

		document.prevButton.src = "images/leftarrow_small_button_p.png";
		document.prevButton.src = "images/leftarrow_small_button.png";

		document.nextButton.src = "images/rightarrow_small_button_p.png";
		document.nextButton.src = "images/rightarrow_small_button.png";

		document.playButton.src = "images/reconnect_small_button_p.png";
		document.playButton.src = "images/reconnect_small_button.png";

		document.gaugeLine.src = "images/gauge_animation1.gif";
		document.gaugeLine.src = "images/gauge1.gif";

		document.stationP0.src = "images/stationMarker_on.gif";
		document.stationP0.src = "images/stationMarker_off.gif";

		document.sleepSw.src = "images/sw_sleep_small_off_f.png";
		document.sleepSw.src = "images/sw_sleep_small_on.png";
		document.sleepSw.src = "images/sw_sleep_small_off.png";
		document.shiftTglSw.src = "images/sw_shift_small_off_f.png";
		document.shiftTglSw.src = "images/sw_shift_small_on.png";
		document.shiftTglSw.src = "images/sw_shift_small_off.png";
		document.sinTglSw.src = "images/sw_sin_small_off_f.png";
		document.sinTglSw.src = "images/sw_sin_small_on.png";
		document.sinTglSw.src = "images/sw_sin_small_off.png";
		document.cwTglSw.src = "images/sw_cw_small_off_f.png";
		document.cwTglSw.src = "images/sw_cw_small_on.png";
		document.cwTglSw.src = "images/sw_cw_small_off.png";
		document.noiseTglSw.src = "images/sw_noise_small_off_f.png";
		document.noiseTglSw.src = "images/sw_noise_small_on.png";
		document.noiseTglSw.src = "images/sw_noise_small_off.png";
		document.helpSw.src = "images/sw_help_small_on.png";
		document.helpSw.src = "images/sw_help_small_off.png";

		document.sleepTimerIndicator.src = "images/anim_indicator.gif";
		document.sleepTimerIndicator.src = "images/indicator_off.png";
		document.shiftSwIndicator.src = "images/indicator_on.png";
		document.shiftSwIndicator.src = "images/indicator_off.png";

		var item = document.getElementById ("gb_0_0").firstChild;
		if ( item ) {
			item.nextSibling.src = "images/radioButton0_focused.png";
			item.nextSibling.src = "images/radioButton0_on.png";
			item.nextSibling.src = "images/radioButton0_off.png";
		}
	}
}

/*----*/
function jumpToShoutcast () {
	refPage = window.open ("http://www.shoutcast.com/disclaimer",
						   "What is SHOUTcast");
	if ( refPage ) refPageIsClosed = false;
	else refPageIsClosed = true;
}

/*----*/
function onUnLoadProc () {
	if ( ! refPageIsClosed ) {
		refPage.close ();
		refPageIsClosed = true;
	}
}

// --------------------------------------------------------------------------
// 放送局名表示でクリックされた場合
// --------------------------------------------------------------------------

function s_mouseDown () {
	if ( bBusy ) return;
	var stationUrl;
	if ( isPSPRadio && psp ) {
		stationUrl = psp.sysRadioGetRelatedPageUrl ();
	}
	else stationUrl = "http://www.scei.co.jp";
	if ( 7 < stationUrl.length ) {
		refPage = window.open (stationUrl, "STATION INFO PAGE");
		if ( refPage ) refPageIsClosed = false;
		else refPageIsClosed = true;
	}
}

var numTableRaw = 10;
var numTableCol = 5;

// --------------------------------------------------------------------------
// 検索キーワード・ジャンル兼用配列
// --------------------------------------------------------------------------

var jpopGenreNum = 27;
var kpopGenreNum = 28;
var variousGenreNum = 49;
genreList = new Array (
	"50s",
	"60s",
	"70s",
	"80s",
	"90s",
	"2000s",
	"Alt",
	"Ambient",
	"Ballade",
	"Blues",
	"Bossa Nova",
	"British",
	"Classical",
	"Contempo",
	"Country",
	"Eclectic",
	"Electronica",
	"Euro",
	"Folk",
	"Game",
	"HardRock",
	"Hawaiian",
	"House",
	"Indie",
	"International",
	"Island",
	"Jazz",
	"JPOP",
	"KPOP",
	"Latin",
	"Live",
	"Metal",
	"Modern",
	"News",
	"Oldies",
	"Pakistani",
	"Polskie",
	"Pop",
	"Progressive",
	"Public",
	"R&B",
	"Reggae",
	"Religious",
	"Salsa",
	"Talk",
	"Techno",
	"Top40",
	"Turkish",
	"Urban",
	"Various"
);

// --------------------------------------------------------------------------
// SHOUTcast の各種 URL
// --------------------------------------------------------------------------

var devID="sh1ODSQoScMFgZWn";
var shoutcastYelloPageUrl_A_API2="http://api.shoutcast.com/legacy/stationsearch?k="+devID+"&search=";
var shoutcastPlsUrl_A_API2="http://yp.shoutcast.com/sbin/tunein-station.pls?id=";
var shoutcastPlsUrl_B_API2="&k="+devID;

var shoutcastYelloPageUrl_A_API1="http://www.shoutcast.com/sbin/newxml.phtml?search=";
var shoutcastPlsUrl_A_API1="http://www.shoutcast.com/sbin/shoutcast-playlist.pls?rn=";
var shoutcastPlsUrl_B_API1="&file=filename.pls";

var shoutcastYelloPageUrl_A=shoutcastYelloPageUrl_A_API2;
var shoutcastPlsUrl_A=shoutcastPlsUrl_A_API2;
var shoutcastPlsUrl_B=shoutcastPlsUrl_B_API2;

var numStationList = 0;
var currentStation = 0;

// --------------------------------------------------------------------------
// 選局ボタンのイベントハンドラ
// --------------------------------------------------------------------------

function b_mouseOver ( sel ) {
	if ( bBusy ) return;
	switch ( sel ) {
	case -1: // Previous
		if ( numStationList <= 1 ) break; 
		document.prevButton.src = "images/leftarrow_small_button_p.png";
		break;
	case 1: // Next
		if ( numStationList <= 1 ) break; 
		document.nextButton.src = "images/rightarrow_small_button_p.png";
		break;
	case 0: // Play
	default:
		if ( numStationList <= 0 ) break; 
		document.playButton.src = "images/reconnect_small_button_p.png";
		break;
	}
}

/*----*/
function b_mouseOut ( sel ) {
	if ( bBusy ) return;
	switch ( sel ) {
	case -1: // Previous
		if ( numStationList <= 1 ) break; 
		document.prevButton.src = "images/leftarrow_small_button.png";
		break;
	case 1: // Next
		if ( numStationList <= 1 ) break; 
		document.nextButton.src = "images/rightarrow_small_button.png";
		break;
	case 0: // Play
	default:
		if ( numStationList <= 0 ) break; 
		document.playButton.src = "images/reconnect_small_button.png";
		break;
	}
}

/*----*/
function b_mouseDown ( sel ) {
	if ( bBusy ) return;
	switch ( sel ) {
	case -1: // Previous
		if ( numStationList <= 1 ) break; 
		psp.sysRadioPlayEffectSound ();
		document.prevButton.src = "images/leftarrow_small_button_p.png";
		currentStation = (currentStation + numStationList - 1)
			% numStationList;
		bIgnoreAutoTuneInToTopStationWhenGotStationList = false;
		tune (currentStation, 0);
		writeIntoCookie ();
		break;
	case 1: // Next
		if ( numStationList <= 1 ) break; 
		psp.sysRadioPlayEffectSound ();
		document.nextButton.src = "images/rightarrow_small_button_p.png";
		currentStation = (currentStation + 1) % numStationList;
		bIgnoreAutoTuneInToTopStationWhenGotStationList = false;
		tune (currentStation, 0);
		writeIntoCookie ();
		break;
	case 0: // Reconnect
	default:
		if ( numStationList <= 0 ) break; 
		psp.sysRadioPlayEffectSound ();
		document.playButton.src = "images/reconnect_small_button_p.png";
		bIgnoreAutoTuneInToTopStationWhenGotStationList = false;
		tune (currentStation, 0);
		writeIntoCookie ();
		break;
	}
}

/*----*/
function b_mouseUp ( sel ) {
}

// --------------------------------------------------------------------------
// 放送局直接選択
// --------------------------------------------------------------------------

function t_mouseDown ( num ) {
	if ( numStationList <= num ) return;
	if ( num == currentStation ) return;
	psp.sysRadioPlayEffectSound ();
	currentStation = num;
	tune (currentStation, 0);
}

// --------------------------------------------------------------------------
// ジャンル切り替えと放送局リストの取得
// --------------------------------------------------------------------------

function g_mouseOver ( rowNum, colNum ) {
	itemName = "gb_"+rowNum+"_"+colNum;
	var genreNum = rowNum + numTableRaw * colNum;
	var item = document.getElementById (itemName).firstChild;
	if ( selectedGenre != genreNum ) {
		item.nextSibling.src = "images/radioButton0_focused.png";
	}
}

/*----*/
function g_mouseOut ( rowNum, colNum ) {
	itemName = "gb_"+rowNum+"_"+colNum;
	var item = document.getElementById (itemName).firstChild;
	var genreNum = rowNum + numTableRaw * colNum;
	if ( selectedGenre == genreNum ) {
		item.nextSibling.src = "images/radioButton0_on.png";
	}
	else {
		item.nextSibling.src = "images/radioButton0_off.png";
	}
}

/*----*/
var bNowHttpGetIsBusy = false;
function g_mouseDown ( rowNum, colNum ) {
	var _selectedGenre = rowNum + numTableRaw * colNum;
	if ( _selectedGenre != selectedGenre ) {
		psp.sysRadioPlayEffectSound ();
		selectedGenre = _selectedGenre;
		bCollectingStations = true;
		messageState = 0; dispMessage (); // SCANNING...
		document.getElementById ("stationInfo").firstChild.nodeValue
			= "“" + unescape (genreList [selectedGenre]) + "”";
		clearGenreListSelection ();
		prepareForGetStationList ();
		bIgnoreAutoTuneInToTopStationWhenGotStationList = false;
		// 文字列解析中（最も重い処理の一つ）の場合は、それを中断させる
		if ( bInAnalizingStationListString ) bForcedExitFlag = true;
		setStationMarker (0);
	}
}

/*----*/
function g_mouseUp ( rowNum, colNum ) {
}

/*----*/
function clearGenreListSelection () {
	for ( i = 0; i < numTableRaw; i++ ) {
		for ( j = 0; j < numTableCol; j++ ) {
			var itemName = "gb_"+i+"_"+j;
			var item = document.getElementById (itemName).firstChild;
			var genreNum = j * numTableRaw + i;
			if ( genreNum == selectedGenre ) {
				item.nextSibling.src = "images/radioButton0_on.png";
			}
			else {
				item.nextSibling.src = "images/radioButton0_off.png";
			}
		}
	}
}

/*----*/
var bLabelInited = false;
function initGenreLabel () {
	if ( bLabelInited ) return;
	bLabelInited = true;
	{
		var today = new Date ();
		var _month = today.getMonth ();
		var _date = today.getDate ();
		// 12月20日から12月25日まで
		if ( 11 == _month && 20 <= _date && _date <= 25 )
			genreList [variousGenreNum] = "Xmas";
		delete today;
	}
	for ( i = 0; i < numTableRaw; i++ ) {
		for ( j = 0; j < numTableCol; j++ ) {
			itemName = "gb_"+i+"_"+j;
			var item = document.getElementById (itemName).firstChild;
			item = item.nextSibling;
			item = item.nextSibling;
			var genreNum = j * numTableRaw + i;
			item.nodeValue = genreList [genreNum];
		}
	}
}

// --------------------------------------------------------------------------
// スリープ処理
// --------------------------------------------------------------------------

function hideSleepMessageProc () {
	bGoingToSleep = false; dispMessage (); // - going to switch to sleep
	clearTimeout (timerID_for_hideSleepMessageProc);
	timerID_for_hideSleepMessageProc = 0;
}

function sleepCountdownProc () {
	clearTimeout (timerID_for_sleepCountDownProc);
	timerID_for_sleepCountDownProc = 0;
	var deltaSec;
	// 現在時刻を取得
	var now = new Date ();
	var currentSec = now.getTime () / 1000;
	delete now;
	deltaSec = currentSec - prevSleepCountDownProcTimestamp;
	if ( sleepCountDown - deltaSec < -10 ) {
		// 接続エラーの検出
		var result = psp.sysRadioGetPlayerStatus ();
		var bLocalErrorDetector = false;
		if ( result != 1 ) {
			bLocalErrorDetector = true;
		}
		// 接続エラー痕跡があり、かつ前回の処理時刻から大きく隔たりがあり、
		// かつスリープタイマー発動予定時刻を既に越えている場合には
		// スリープタイマーを解除する
		// 注）手動でサスペンド（スリープタイマーで設定された時間より長く）→レジューム
		// 　　操作が行われた場合等にスリープタイマーを解除するため。
		// 　　ただし、HOME ボタンによって XMB 表示を行っているときに受信エラーが発生
		// 　　した場合などでは、サスペンド→レジュームが行われたと誤認識する可能性がある。
		// 　　（サスペンド→レジュームを検出する JavaScript メソッドを提供するまでは
		// 　　　この不確かな処理方法で。。）
		if ( bErrorDetectorForSleepCountDownProc
			 || bLocalErrorDetector ) {
			bSleepTimerWasActivated = false;
			document.sleepTimerIndicator.src = "images/indicator_off.png";
			document.sleepSw.src = "images/sw_sleep_small_off.png";
			sleepCountDown = 0;
			dispSleepCountDown ();
			if ( 0 < timerID_for_hideSleepMessageProc ) {
				clearTimeout (timerID_for_hideSleepMessageProc);
				timerID_for_hideSleepMessageProc = 0;
			}
			bGoingToSleep = false; dispMessage (); // - going to switch to sleep
			return;
		}
	}
	prevSleepCountDownProcTimestamp = currentSec;
	sleepCountDown -= Math.floor (deltaSec);
	var next = 60000;
	// sysRadioSleep メソッドコール後約 10 秒でスリープするので、
	// 最後の１分近傍は少し短く設定する
	if ( 10 < sleepCountDown && sleepCountDown <= 70 ) {
		next = sleepCountDown * 1000 - (10 * 1000);
		bGoingToSleep = true; dispMessage (); // going to switch to sleep
	}
	else if ( sleepCountDown <= 10 ) {
		bGoingToSleep = true; dispMessage (); // going to switch to sleep
		sleepCountDown = 0;
		bSleepTimerWasActivated = false;
		document.sleepTimerIndicator.src = "images/indicator_off.png";
		document.sleepSw.src = "images/sw_sleep_small_off.png";
		psp.sysRadioSleep (); /* このメソッドコールを行うと
								 約 10 秒後にスリープする */
		
	}

	if ( 0 < timerID_for_hideSleepMessageProc ) {
		clearTimeout (timerID_for_hideSleepMessageProc);
		timerID_for_hideSleepMessageProc = 0;
	}
	timerID_for_hideSleepMessageProc
		= setTimeout ('hideSleepMessageProc ()',
					  (sleepCountDown <= 0) ? 10000 : (next + 10000));
	dispSleepCountDown ();
	bErrorDetectorForSleepCountDownProc = false;
	if ( bSleepTimerWasActivated )
		timerID_for_sleepCountDownProc
			= setTimeout ('sleepCountdownProc ()', next);
}

/*----*/
function dispSleepCountDown () {
	if ( ! bSleepTimerWasActivated || sleepCountDown <= 0 )
		document.getElementById ("sleepCountDown").firstChild.nodeValue = "";
	else {
		var sleepCountDown_HH
			= Math.floor (sleepCountDown / 3600);
		var sleepCountDown_MM
			= Math.floor (((sleepCountDown % 3600) + 30) / 60);
		if ( sleepCountDown_MM < 10 )
			document.getElementById ("sleepCountDown").firstChild.nodeValue
				= sleepCountDown_HH + ":0" + sleepCountDown_MM;
		else
			document.getElementById ("sleepCountDown").firstChild.nodeValue
				= sleepCountDown_HH + ":" + sleepCountDown_MM;
	}
}

// --------------------------------------------------------------------------
// 下部のトグルスイッチ/プッシュスイッチ
// --------------------------------------------------------------------------

function sw_sleep ( mode ) {
	switch ( mode ) {
	case 0: // mouse over
		document.sleepSw.src
			= bSleepTimerWasActivated
			? "images/sw_sleep_small_on.png"
			: "images/sw_sleep_small_off_f.png";
		break;
	case 1: // mouse out
		document.sleepSw.src
			= bSleepTimerWasActivated
			? "images/sw_sleep_small_on.png"
			: "images/sw_sleep_small_off.png";
		break;
	case 2: // mouse down
	default:
		psp.sysRadioPlayEffectSound ();
		if ( ! bSleepSupport ) { /* システムソフトウェアが 3.93 よりも古いシステムでは
									sysRadioSleep は使えない */
			alert (messageList [langID] [7]);
			break;
		}
		if ( timerID_for_sleepCountDownProc ) {
			clearTimeout (timerID_for_sleepCountDownProc);
			timerID_for_sleepCountDownProc = 0;
		}
		if ( bGoingToSleep ) {
			bGoingToSleep = false; dispMessage (); // - going to switch to sleep
		}
		if ( 0 < timerID_for_hideSleepMessageProc ) {
			clearTimeout (timerID_for_hideSleepMessageProc);
			timerID_for_hideSleepMessageProc = 0;
		}
		sleepCountDown
			= Math.floor (sleepCountDown / 900) * 900 + 900;
		if ( 7200 < sleepCountDown ) {
			sleepCountDown = 0;
			document.sleepTimerIndicator.src = "images/indicator_off.png";
			bSleepTimerWasActivated = false;
		}
		else {
			if ( ! bSleepTimerWasActivated ) {
				bSleepTimerWasActivated = true;
				document.sleepTimerIndicator.src = "images/anim_indicator.gif";
			}
			bErrorDetectorForSleepCountDownProc = false;
			// 現在時刻を取得
			var now = new Date ();
			prevSleepCountDownProcTimestamp = now.getTime () / 1000;
			delete now;
			timerID_for_sleepCountDownProc
				= setTimeout ('sleepCountdownProc ()', 60000);
		}
		document.sleepSw.src
			= bSleepTimerWasActivated
			? "images/sw_sleep_small_on.png"
			: "images/sw_sleep_small_off_f.png";
		dispSleepCountDown ();
		break;
	}
}

function sw_shift ( mode ) {
	switch ( mode ) {
	case 0: // mouse over
		if ( sw_shift_status == 0 )
			document.shiftTglSw.src = "images/sw_shift_small_off_f.png";
		break;
	case 1: // mouse out
		if ( sw_shift_status == 0 ) {
			document.shiftTglSw.src = "images/sw_shift_small_off.png";
		}
		else {
			document.shiftTglSw.src = "images/sw_shift_small_on.png";
		}
		break;
	case 2: // mouse down
	default:
		psp.sysRadioPlayEffectSound ();
		writeIntoCookie ();
		if ( sw_shift_status == 0 ) {
			sw_shift_status = 1;
			document.shiftTglSw.src = "images/sw_shift_small_on.png";
			document.shiftSwIndicator.src = "images/indicator_on.png";
		}
		else {
			sw_shift_status = 0;
			document.shiftTglSw.src = "images/sw_shift_small_off_f.png";
			document.shiftSwIndicator.src = "images/indicator_off.png";
			psp.sysRadioSetAudioShiftWidth (0);
		}
		break;
	}
}

function sw_sin ( mode ) {
	switch ( mode ) {
	case 0: // mouse over
		if ( sw_sin_status == 0 )
			document.sinTglSw.src = "images/sw_sin_small_off_f.png";
		break;
	case 1: // mouse out
		if ( sw_sin_status == 0 ) {
			document.sinTglSw.src = "images/sw_sin_small_off.png";
		}
		else {
			document.sinTglSw.src = "images/sw_sin_small_on.png";
		}
		break;
	case 2: // mouse down
	default:
		psp.sysRadioPlayEffectSound ();
		writeIntoCookie ();
		if ( sw_sin_status == 0 ) {
			sw_sin_status = 1;
			document.sinTglSw.src = "images/sw_sin_small_on.png";
			document.sinSwIndicator.src = "images/indicator_on.png";
		}
		else {
			sw_sin_status = 0;
			document.sinTglSw.src = "images/sw_sin_small_off_f.png";
			document.sinSwIndicator.src = "images/indicator_off.png";
			// CW も OFF にする
			sw_cw_status = 0;
			document.cwTglSw.src = "images/sw_cw_small_off.png";
			document.cwSwIndicator.src = "images/indicator_off.png";
			psp.sysRadioSetSineWaveOscillatorFrequencyAndVolume
				(8000, 0, sinWaveTension, "");
		}
		break;
	}
}

function sw_cw ( mode ) {
	switch ( mode ) {
	case 0: // mouse over
		if ( sw_cw_status == 0 )
			document.cwTglSw.src = "images/sw_cw_small_off_f.png";
		break;
	case 1: // mouse out
		if ( sw_cw_status == 0 ) {
			document.cwTglSw.src = "images/sw_cw_small_off.png";
		}
		else {
			document.cwTglSw.src = "images/sw_cw_small_on.png";
		}
		break;
	case 2: // mouse down
	default:
		psp.sysRadioPlayEffectSound ();
		writeIntoCookie ();
		if ( sw_cw_status == 0 ) {
			sw_cw_status = 1;
			document.cwTglSw.src = "images/sw_cw_small_on.png";
			document.cwSwIndicator.src = "images/indicator_on.png";
			// sin も ON にする
			sw_sin_status = 1;
			document.sinTglSw.src = "images/sw_sin_small_on.png";
			document.sinSwIndicator.src = "images/indicator_on.png";
		}
		else {
			sw_cw_status = 0;
			document.cwTglSw.src = "images/sw_cw_small_off_f.png";
			document.cwSwIndicator.src = "images/indicator_off.png";
		}
		break;
	}
}

function sw_noise ( mode ) {
	switch ( mode ) {
	case 0: // mouse over
		if ( sw_noise_status == 0 )
			document.noiseTglSw.src = "images/sw_noise_small_off_f.png";
		break;
	case 1: // mouse out
		if ( sw_noise_status == 0 ) {
			document.noiseTglSw.src = "images/sw_noise_small_off.png";
		}
		else {
			document.noiseTglSw.src = "images/sw_noise_small_on.png";
		}
		break;
	case 2: // mouse down
	default:
		psp.sysRadioPlayEffectSound ();
		writeIntoCookie ();
		if ( sw_noise_status == 0 ) {
			sw_noise_status = 1;
			document.noiseTglSw.src = "images/sw_noise_small_on.png";
			document.noiseSwIndicator.src = "images/indicator_on.png";
		}
		else {
			sw_noise_status = 0;
			document.noiseTglSw.src = "images/sw_noise_small_off_f.png";
			document.noiseSwIndicator.src = "images/indicator_off.png";
			psp.sysRadioSetWhiteNoiseOscillatorVolume (0);
		}
		break;
	}
}

function sw_help ( mode ) {
	switch ( mode ) {
	case 0: // mouse over
		document.helpSw.src = "images/sw_help_small_on.png";
		break;
	case 1: // mouse out
		document.helpSw.src = "images/sw_help_small_off.png";
		break;
	case 2: // mouse down
	default:
		psp.sysRadioPlayEffectSound ();
		document.helpSw.src = "images/sw_help_small_off.png";
		refPage = window.open
			("./usage/" + usageLang + "/index.html", "STATION INFO PAGE");
		if ( refPage ) refPageIsClosed = false;
		else refPageIsClosed = true;
		break;
	}
}

// --------------------------------------------------------------------------
// 放送局リスト取得のための準備
// --------------------------------------------------------------------------

function prepareForGetStationList () {
	if ( 0 <= selectedGenre ) {
		bBusy = true;
		if ( isPSPRadio && psp ) psp.sysRadioBusyIndicator (1);
		// ゲージのアニメーションを開始
		document.gaugeLine.src = "images/gauge_animation1.gif";
		// 検索キーワード
		var url = shoutcastYelloPageUrl_A
			+ escape (genreList [selectedGenre]);
		if ( isPSPRadio && psp ) {
			var size = 16384;
			psp.sysRadioPrepareForHttpGet (url, userAgentForHttpGet, size, 0);
		}
		bNowHttpGetIsBusy = true;
		httpGetProc ();
	}
}

/*----*/
var dummyStationList
="<?xml version=\"1.0\" encoding=\'UTF-8\' standalone=\"yes\"?>\n"
+"<stationlist>\n"
+"<tunein base=\"/sbin/tunein-station.pls\"></tunein>\n"
+"</stationlist>";

var stationArray = new Array (0);

/*----*/
function httpGetProc () {
	if ( 0 < timerID_for_httpGetProc ) {
		clearTimeout (timerID_for_httpGetProc);
		timerID_for_httpGetProc = 0;
	}
	if ( ! bNowHttpGetIsBusy ) {
		return;
	}
	var stationListStr;
	if ( isPSPRadio && psp ) {
        result = psp.sysRadioGetHttpGetStatus ();
        if ( result == 1 || result == -1 ) {
			timerID_for_httpGetProc
				= setTimeout ('httpGetProc ()', 500);
			return; // error;
		}
		// result == 0 の場合は HttpGet 処理が完了していて、データの読み込みが可能
		stationListStr = psp.sysRadioGetHttpGetResult ();
		psp.sysRadioHttpGetTerminate ();
	}
	else {
		stationListStr = dummyStationList;
	}
	delete stationArray;
	stationArray = makeStationList (stationListStr);

	var n = stationArray.length;
	if ( maxNumStation < n ) {
		n = maxNumStation;
		stationArray = stationArray.slice (0, maxNumStation);
	}
	numStationList = n;

	// ラジオプレイヤー起動直後の場合は、
	// クッキーから取得した前回の最後にプレイしていた放送局が配列中に
	// 存在する場合は、そこを対象とする。
	// 存在しない場合は、先頭に追加し、それを対象とする。
	if ( bIgnoreAutoTuneInToTopStationWhenGotStationList ) {
		var i, N=(stationArray.length);
		var bFoundIt=false;
		// 配列中から探す
		for ( i = 0; i < N; i++ ) {
			if ( ( lastTimeStationInfo.stationName
				   == stationArray [i].stationName )
				 && ( lastTimeStationInfo.br
					  == stationArray [i].br )
				 && ( lastTimeStationInfo.sr
					  == stationArray [i].sr )
				 && ( lastTimeStationInfo.aacp
					  == stationArray [i].aacp ) ) {
				bFoundIt = true;
				break;
			}
		}
		// 見つかった場合
		if ( bFoundIt ) {
			currentStation = i;
			setStationMarkerPos ();
			setScaleHand (currentStation);
		}
		// 見つからなかった場合
		else {
			// 配列の先頭に追加
			stationArray.unshift
				({stationName: lastTimeStationInfo.stationName,
					  comment: lastTimeStationInfo.comment,
						   id: "",
						   lc: "0",
						   br: lastTimeStationInfo.br,
						   sr: lastTimeStationInfo.sr,
					     aacp: lastTimeStationInfo.aacp,
					  refPage: lastTimeStationInfo.refPage,
					streamUrl: lastTimeStationInfo.streamUrl
						});
			currentStation = 0;
			setStationMarkerPos ();
			++numStationList;
			if ( maxNumStation < numStationList ) {
				stationArray = stationArray.slice (0, maxNumStation);
				numStationList = maxNumStation;
			}
		}
	}
	// とりあえずリスト先頭の放送局にチューニングする
	else {
		currentStation = 0;
		setStationMarkerPos ();
	}

	// 取得に失敗した場合
	if ( stationArray.length == 0 ) {
		// 強制中断でない場合
		if ( bForcedExitFlag == false ) {
			// 放送局が存在しない
			if ( isPSPRadio && psp ) {
				if ( sw_noise_status == 1 )
					psp.sysRadioSetWhiteNoiseOscillatorVolume (80);
				psp.sysRadioSetSubVolume (0);	
				psp.sysRadioStop ();
				psp.sysRadioBusyIndicator (0);
				// ゲージのアニメーションを停止
				document.gaugeLine.src = "images/gauge1.gif";
			}
			bNowHttpGetIsBusy = false;
		}
		else { // 強制中断された場合
			bForcedExitFlag = false;
			timerID_for_httpGetProc
				= setTimeout ('httpGetProc ()', 500);
		}
		currentStation = 0;
		numStationList = 0;
		return;
	}
	bForcedExitFlag = false;

	// ステーションマーカーをセット
	setStationMarker (stationArray.length);

	if ( isPSPRadio && psp ) psp.sysRadioBusyIndicator (0);

	if ( ! bIgnoreAutoTuneInToTopStationWhenGotStationList ) {
		tune (currentStation, 0);
		messageState = 1; dispMessage (); // CONNECTING...
		bCollectingStations = false;
	}
	else bIgnoreAutoTuneInToTopStationWhenGotStationList = false;
	writeIntoCookie ();
	bBusy = false;
	bNowHttpGetIsBusy = false;
}

/*----*/
function tune ( stationNumber, mode ) {
	if ( ! refPageIsClosed ) {
		refPage.close ();
		refPageIsClosed = true;
	}
	if ( numStationList == 0 ) { // 放送局が存在しない場合
		if ( isPSPRadio && psp ) {
			if ( sw_noise_status == 1 )
				psp.sysRadioSetWhiteNoiseOscillatorVolume (60);
			psp.sysRadioSetSubVolume (0);
			psp.sysRadioStop ();
		}
		messageState = 3; dispMessage (); // NO STATION
		streamStatusCheckProcWorkState = 0;
		return;
	}
	if ( numStationList <= stationNumber ) return;
	if ( stationArray [stationNumber].id.length == 0 ) mode = 1;

	bConnectError = true; // 一旦フラグを立てる
	if ( isPSPRadio && psp ) {
		streamStatusCheckProcWorkState = 0;
		// ゲージのアニメーションを開始
		document.gaugeLine.src = "images/gauge_animation1.gif";
		psp.sysRadioBusyIndicator (1);
		messageState = 1; dispMessage (); // CONNECTING...
		document.getElementById ("stationInfo").firstChild.nodeValue
			= psp.sysRadioCharacterEntityConvert
			(unescape (stationArray [stationNumber].stationName));
		streamTitle = "";
		if ( sw_sin_status == 1 ) {
			if ( sw_cw_status == 1 ) {
				psp.sysRadioSetSineWaveOscillatorFrequencyAndVolume
					(320 + Math.random () * 1600, 40, sinWaveTension,
					 stationArray [stationNumber].stationName,
					 2 + Math.random () * 5);
			}
			else {
				psp.sysRadioSetSineWaveOscillatorFrequencyAndVolume
					(320 + Math.random () * 1600, 40, sinWaveTension);
			}
		}
		if ( sw_shift_status == 1 ) {
			if ( Math.random () < 0.5 ) psp.sysRadioSetAudioShiftWidth (-25);
			else psp.sysRadioSetAudioShiftWidth (15);
		}
		if ( mode == 0 ) { // PLS の URL 指定で再生
			var stationID = stationArray [stationNumber].id;
			var plsURL = shoutcastPlsUrl_A + stationID + shoutcastPlsUrl_B;
			psp.sysRadioPlayPls
				(plsURL, userAgentForGetPls, userAgentForPlayStream);
		}
		else { // ストリーム URL 指定で再生
			psp.sysRadioPlayStream
				(stationArray [stationNumber].streamUrl,
				 userAgentForPlayStream);
		}
		setScaleHand (stationNumber);
		psp.sysRadioSetSubVolume (255);
	}
	else {}
}

/*----*/
function setStationMarkerPos () {
	var i, m = 1.0 / (maxNumStation - 1.0);
	for ( i = 0; i < stationArray.length; i++ ) {
		stationArray [i].gaugePos = i * m;
	}
}

/*----*/
function setScaleHand ( num ) {
	if ( 0 <= num ) {
		document.gaugeHand.style.left
			= 19 - 30/2 + 5
			+ stationArray [num].gaugePos * (15 * (maxNumStation - 1));
	}
	else {
		document.gaugeHand.style.left
			= 19 - 30/2 + 5;
	}
}

// --------------------------------------------------------------------------
// ステーションリスト文字列を解析し、放送局リスト（配列）を作る
// --------------------------------------------------------------------------

var keyword_EndOfQuote = "\"";
var keyword_Station = "<station ";
var keyword_name = "name=\"";
var keyword_mt = "mt=\"";
var keyword_id = "id=\"";
var keyword_br = "br=\"";
var keyword_genre = "genre=\"";
var keyword_lc = "lc=\"";

function makeStationList ( stationListString ) {
    stationRec = new Object ();
    var currentPos = 0;
	var startPos = 0;
	var endPos = 0;
    var prevCurrentPos = -1;
    var state = 0;
    var count = 0;
	psp.sysRadioPrepareForStrOperation (stationListString);
    var length = psp.sysRadioStrLength ();
    var bExit = false;
    stationList = new Array (0);
	bInAnalizingStationListString = true;
    while ( bExit == false && bForcedExitFlag == false ) {
		switch ( state ) {
		case 0: // name
			stationRec.id = stationRec.br = stationRec.sr = stationRec.lc = 0;
			stationRec.mt = stationRec.genre = stationRec.rp = "";
			startPos = psp.sysRadioStrIndexOf (keyword_name, currentPos);
			if ( startPos < 0 || currentPos == startPos ) {
				bExit = true;
				break;
			}
			startPos += keyword_name.length;
			endPos = psp.sysRadioStrIndexOf
				(keyword_EndOfQuote, startPos);
			stationRec.name = psp.sysRadioStrSlice (startPos, endPos);
			currentPos = endPos + keyword_EndOfQuote.length;
			++state;
			break;
		case 1: // mt
			startPos = psp.sysRadioStrIndexOf (keyword_mt, currentPos);
			if ( startPos < 0 || currentPos == startPos ) {
				bExit = true;
				break;
			}
			startPos += keyword_mt.length;
			endPos = psp.sysRadioStrIndexOf (keyword_EndOfQuote, startPos);
			stationRec.mt = psp.sysRadioStrSlice (startPos, endPos);
			currentPos = endPos + keyword_EndOfQuote.length;
			++state;
			break;
		case 2: // id
			startPos = psp.sysRadioStrIndexOf (keyword_id, currentPos);
			if ( startPos < 0 || currentPos == startPos ) {
				bExit = true;
				break;
			}
			startPos += keyword_id.length;
			endPos = psp.sysRadioStrIndexOf (keyword_EndOfQuote, startPos);
			stationRec.id = psp.sysRadioStrSlice (startPos, endPos);
			currentPos = endPos + keyword_EndOfQuote.length;
			++state;
			break;
		case 3: // br (24, 28, 32, 40, 48, 56, 64, 96, 112, 128, 160, 192, 320)
			startPos = psp.sysRadioStrIndexOf (keyword_br, currentPos);
			if ( startPos < 0 || currentPos == startPos ) {
				bExit = true;
				break;
			}
			startPos += keyword_br.length;
			endPos = psp.sysRadioStrIndexOf (keyword_EndOfQuote, startPos);
			stationRec.br = psp.sysRadioStrSlice (startPos, endPos);
			currentPos = endPos + keyword_EndOfQuote.length;
			++state;
			break;
		case 4: // genre
			startPos = psp.sysRadioStrIndexOf (keyword_genre, currentPos);
			if ( startPos < 0 || currentPos == startPos ) {
				bExit = true;
				break;
			}
			startPos += keyword_genre.length;
			endPos = psp.sysRadioStrIndexOf (keyword_EndOfQuote, startPos);
			stationRec.genre = psp.sysRadioStrSlice (startPos, endPos);
			currentPos = endPos + keyword_EndOfQuote.length;
			++state;
			break;
		case 5: // lc
			startPos = psp.sysRadioStrIndexOf (keyword_lc, currentPos);
			if ( startPos < 0 || currentPos == startPos ) {
				bExit = true;
				break;
			}
			startPos += keyword_lc.length;
			endPos = psp.sysRadioStrIndexOf (keyword_EndOfQuote, startPos);
			stationRec.lc = psp.sysRadioStrSlice (startPos, endPos);
			currentPos = endPos + keyword_EndOfQuote.length;
			++state;
			break;
		case 6:
			if ( ( currentPos < prevCurrentPos )
				 || ( prevCurrentPos == currentPos )
				 || ( length - 1 <= currentPos ) ) {
				bExit = true;
			}
			else {
				if ( ( bAacpSupport && stationRec.mt == "audio/aacp" )
					 || stationRec.mt == "audio/mpeg" ) {
					stationList.push
						({stationName: stationRec.name,
							  comment: stationRec.genre,
								   id: stationRec.id,
								   lc: stationRec.lc,
								   br: stationRec.br,
								   sr: stationRec.sr,
								 aacp: (stationRec.mt == "audio/aacp")
								       ? true : false,
							  refPage: stationRec.rp,
							streamUrl: ""
								});
					++count;
					if ( maxNumStation <= count ) bExit = true;
				}
			}
			prevCurrentPos = currentPos;
			state = 0;
		}
    }
	psp.sysRadioStrOperationTerminate ();

	// 強制中断された場合は、配列を一旦破棄し空の配列を返す。
	if ( bForcedExitFlag ) {
		delete stationList;
		stationList = new Array (0);
	}
	bInAnalizingStationListString = false;
	delete stationRec;

    return ( stationList );
}

// --------------------------------------------------------------------------
// アナログスティック
// --------------------------------------------------------------------------

function analogStickProc () {
	return;
    if ( isPSPRadio && psp ) {
		var posX = psp.sysRadioGetAnalogStickXValue ();
		var posY = psp.sysRadioGetAnalogStickYValue ();
		if ( (posX < -5 || 5 < posX)
			 && (posY < -5 || 5 < posY) ) {
			psp.sysRadioSetSineWaveOscillatorFrequencyAndVolume
				((posX*posX + posY*posY)/6, 80);
		}
		else {
			psp.sysRadioSetSineWaveOscillatorFrequencyAndVolume (0, 0);
		}
    }
}

// --------------------------------------------------------------------------
// ストリーム生 URL を取得し、クッキーに書き出す
// --------------------------------------------------------------------------

function updateStreamUrl () {
	if ( 0 < timerID_for_streamUrl ) {
		clearTimeout (timerID_for_streamUrl);
		timerID_for_streamUrl = 0;
	}
	if ( ! bDoUpdateStreamUrl || (! isPSPRadio || ! psp) ) {
		timerID_for_streamUrl = setTimeout ('updateStreamUrl ()', 1000);
		return;
	}
	var streamUrl = psp.sysRadioGetStreamUrl ();
	var expirationDate = new Date ();
	expirationDate.setTime (expirationDate.getTime ()
							+ (365*1000*60*60*24));
	var exDay = expirationDate.toGMTString ();
	delete expirationDate;
	var cookieValue = "";
	if ( 0 < streamUrl.length ) {
		cookieValue
			= cookieValue
			+ "genre:"+ selectedGenre + ";"
			+ "streamUrl:"+ streamUrl + ";"
			+ "stationName:"
			+ stationArray [currentStation].stationName + ";"
			+ "comment:"
			+ stationArray [currentStation].comment + ";"
			+ "refPage:"
			+ stationArray [currentStation].refPage + ";"
			+ "br:"
			+ stationArray [currentStation].br + ";"
			+ "sr:"
			+ stationArray [currentStation].sr + ";"
			+ "aacp:" + stationArray [currentStation].aacp + ";";
		bDoUpdateStreamUrl = false;
	}
	cookieValue
		= cookieValue
		+ "shiftSw:" + sw_shift_status + ";"
		+ "sinSw:" + sw_sin_status + ";"
		+ "cwSw:" + sw_cw_status + ";"
		+ "noiseSw:" + sw_noise_status + ";"
		+ "coreVersion:" + psp.sysRadioGetPlayerCoreVersionInfo () + ";";
	document.cookie
		= "IRP1="+ escape (cookieValue)
		+ ";path=" + location.pathname
		+ ";expires=" + exDay;
	timerID_for_streamUrl = setTimeout ('updateStreamUrl ()', 1000);
}

// --------------------------------------------------------------------------
// ストリーム受信状態の取得 (一定間隔でコールされ続ける)
// --------------------------------------------------------------------------

var connectionTimeoutCountdown = 0;
function streamStatusCheckProc () {
	if ( 0 < timerID_for_streamStatusCheckProc ) {
		clearTimeout (timerID_for_streamStatusCheckProc);
		timerID_for_streamStatusCheckProc = 0;
	}
	if ( ! isPSPRadio || ! psp ) {
		timerID_for_streamStatusCheckProc
			= setTimeout ('streamStatusCheckProc ()', 1500);
		return;
	}
	if ( numStationList == 0 ) {
		timerID_for_streamStatusCheckProc
			= setTimeout ('streamStatusCheckProc ()', 4000);
		return;
	}
	switch ( streamStatusCheckProcWorkState ) {
	case 0:
		{
			var result = psp.sysRadioGetPlayerStatus ();
			switch ( result ) {
			case -1: // エラー
				if ( sw_noise_status == 1 )
					psp.sysRadioSetWhiteNoiseOscillatorVolume (80);
				break;
			case 1: // 再生中
			case 4: // 放送サーバーへ接続中
			case 0: // 処理中でない
				streamStatusCheckProcWorkState = 1;
				connectionTimeoutCountdown = 20;
				break;
			case 2: // pls あるいは m3u データを取得中
			case 3: // pls あるいは m3u データを解析中
			default:
				break;
			}
		}
		break;
	case 1: // 効果音の発生時間を充分にとるための時間稼ぎ
		{
			var result = psp.sysRadioGetPlayerStatus ();
			switch ( result ) {
			case 1: // 再生中
				streamStatusCheckProcWorkState = 2;
				break;
			default:
				// つながりが悪くても暫くはここで待つ
				if ( --connectionTimeoutCountdown <= 0 ) {
					// あきらめて接続エラー扱いとする
					streamStatusCheckProcWorkState = 2;
				}
				break;
			}
		}
		break;
	case 2: // ここで効果音の出力を終了させる
		if ( ! bNowHttpGetIsBusy && ! bInAnalizingStationListString ) {
			streamStatusCheckProcWorkState = 3;
			psp.sysRadioSetSineWaveOscillatorFrequencyAndVolume
				(8000, 0, sinWaveTension, "");
			psp.sysRadioSetSubVolume (255);
			psp.sysRadioSetAudioShiftWidth (0);
			// ストリームタイトル表示の更新のため
			streamTitle = contentMetaInfo = "";
			messageState = 6; dispMessage (); // artist name - song name
		}
		psp.sysRadioSetWhiteNoiseOscillatorVolume (0);
		break;
	case 3: // 再生状態を監視し続ける
	default:
		{
			if ( ! bNowHttpGetIsBusy && ! bInAnalizingStationListString ) {
				// ゲージのアニメーションを停止
				document.gaugeLine.src = "images/gauge1.gif";
			}
			psp.sysRadioStop (1); // 逆側のストリームを強制的に停止させる
			var result = psp.sysRadioGetPlayerStatus ();
			switch ( result ) {
			case -1: // エラー
			case 0: // 処理中でない
				if ( sw_noise_status == 1 )
					psp.sysRadioSetWhiteNoiseOscillatorVolume (80);
				messageState = 2; dispMessage (); // CONNECTION ERROR
				document.getElementById ("stationInfo").firstChild.nodeValue
					= psp.sysRadioCharacterEntityConvert
					(unescape (stationArray [currentStation].stationName));
				streamTitle = "";
				if ( ! bBusy ) psp.sysRadioBusyIndicator (0);
				bConnectError = true;
				bErrorDetectorForSleepCountDownProc = true;
				break;
			case 1: // 再生中
				if ( bConnectError ) {
					contentMetaInfo = "";
					messageState = 6; dispMessage (); // artist name - song name
					bConnectError = false;
				}
				psp.sysRadioSetWhiteNoiseOscillatorVolume (0);
				if ( ! bBusy ) psp.sysRadioBusyIndicator (0);
				break;
			default:
				break;
			}
		}
		break;
	}
	timerID_for_streamStatusCheckProc
	  = setTimeout ('streamStatusCheckProc ()', 1500);
}

// --------------------------------------------------------------------------
// マジックウィンドウ（インジケータ）の表示更新
// --------------------------------------------------------------------------
var prevC=(-1);
var prevB=(-1);
var prevP=(-1);
function updateMagicWindowProc () {
	var m = Math.floor (Math.random () * 3.0);
	if ( isPSPRadio && psp ) {
		var n;
		{
			// 左（音の歪み度合い）
			var value = psp.sysRadioGetAudioSoundDistortionRate ();
			if ( 0 == value ) n = 4;
			else if ( value < 5 ) n = 3;
			else if ( value < 10 ) n = 2;
			else if ( value < 15) n = 1;
			else n = 0;
			if ( prevC != n ) {
				document.leftMWindow.src
					= "images/magic_window0_" + n + ".png";
				prevC = n;
			}
		}
		{
			// 中央（バッファーの満たされ度合い）
			var value = psp.sysRadioGetStreamBufferLevel ();
			if ( 32 < value ) n = 4;
			else if ( 24 < value ) n = 3;
			else if ( 16 < value ) n = 2;
			else if ( 8 < value ) n = 1;
			else n = 0;
			if ( prevB != n ) {
				document.centerMWindow.src
					= "images/magic_window1_" + n + ".png";
				prevB = n;
			}
		}
		{
			// 右（帯域安定度）
			var value = psp.sysRadioGetNetworkBandQuality ();
			if ( 255 == value ) n = 4;
			else if ( 240 < value ) n = 3;
			else if ( 220 < value ) n = 2;
			else if ( 200 < value ) n = 1;
			else n = 0;
			if ( prevP != n ) {
				document.rightMWindow.src
					= "images/magic_window2_" + n + ".png";
				prevP = n;
			}
		}
	}
	else {
		var n = Math.floor (Math.random () * 5.0);
		switch ( m ) {
		case 0:
			document.leftMWindow.src = "images/magic_window0_" + n + ".png";
			break;
		case 1:
			document.centerMWindow.src = "images/magic_window1_" + n + ".png";
			break;
		case 2:
		default:
			document.rightMWindow.src = "images/magic_window2_" + n + ".png";
			break;
		}
	}
}

// --------------------------------------------------------------------------
// スタートボタン（ミュートボタン）の監視
// --------------------------------------------------------------------------

function startButtonPollingProc () {
	if ( isPSPRadio && psp ) {
		var status = psp.sysRadioGetStartButtonToggleStatus ();
		if ( status == 1 && bMute ) {
			bMute = false;
			messageState = 6; dispMessage (); // artist name - song name
			psp.sysRadioSetMasterVolume (255);
			streamTitle = ""; // 強制的に再表示させるため
		}
		else if ( status == 0 && (! bMute) ) {
			bMute = true;
			messageState = 4; dispMessage (); // MUTING...
			psp.sysRadioSetMasterVolume (0);
		}
	}
}

// --------------------------------------------------------------------------
// メッセージ表示
// --------------------------------------------------------------------------

function dispMessage () {
	var msg;
	if ( bGoingToSleep ) msg = messageList [langID] [8]; // going to switch to sleep
	else if ( bMute ) msg = messageList [langID] [4]; // MUTING...
	else if ( messageState <= 5 ) msg = messageList [langID] [messageState];
	else if ( messageState == 6 ) msg = contentMetaInfo; // artist name - song name
	else msg = "";
	var currentFontSize;
	var node = document.getElementById ("metaInfo").firstChild;
	if ( 40 < msg.length ) currentFontSize = "75%";
	else currentFontSize = "100%";
	if ( lastContentMetaInfoFontSize != currentFontSize ) {
		node.nodeValue = "";
		document.getElementById ("metaInfo").style.fontSize
			= lastContentMetaInfoFontSize = currentFontSize;
	}
	node.nodeValue = msg;
}

// --------------------------------------------------------------------------
// コンテントメタインフォ処理
// --------------------------------------------------------------------------

var lastContentMetaInfoFontSize = "100%";
function updateContentMetaInfo () {
	if ( 0 < timerID_for_contentMetaInfo ) {
		clearTimeout (timerID_for_contentMetaInfo);
		timerID_for_contentMetaInfo = 0;
	}
	if ( bCollectingStations
		 || streamStatusCheckProcWorkState < 3
		 || bConnectError ) {
		timerID_for_contentMetaInfo
			= setTimeout ('updateContentMetaInfo ()', 2000);
		return;
	}
	var artistAndSongName;
	var bitRate = 0;
	var sampleRate = 0;
	if ( isPSPRadio && psp ) {
		if ( selectedGenre == kpopGenreNum )
			artistAndSongName = psp.sysRadioGetContentMetaInfo (2);
		else if ( selectedGenre == jpopGenreNum )
			artistAndSongName = psp.sysRadioGetContentMetaInfo (1);
		else
			artistAndSongName = psp.sysRadioGetContentMetaInfo (0);
	}
	else artistAndSongName = "artist name - song name";
	updateStreamTitle ();
	if ( lastArtistAndSongName == artistAndSongName ) {
		timerID_for_contentMetaInfo
			= setTimeout ('updateContentMetaInfo ()', 2000);
		return;
	}
	lastArtistAndSongName = artistAndSongName;
	artistAndSongName = unescape (artistAndSongName);
	contentMetaInfo
		= ((0 < artistAndSongName.length ) ? "♫ " : "")
		+ artistAndSongName;
	dispMessage ();
    timerID_for_contentMetaInfo
		= setTimeout ('updateContentMetaInfo ()', 2000);
}

// --------------------------------------------------------------------------
// ストリームタイトル（放送局名）の表示更新
// --------------------------------------------------------------------------

function updateStreamTitle () {
	if ( bMute ) {
		document.getElementById ("stationInfo").firstChild.nodeValue
			= messageList [langID] [5]; // Push “START” button ...
		return;
	}
	if ( streamTitle != "" ) return;
	var stationName;
	var bitRate = 0;
	var sampleRate = 0;
	if ( isPSPRadio && psp ) {
		stationName = psp.sysRadioGetStreamTitle (0);
		bitRate = psp.sysRadioGetBitRate () / 1000;
		sampleRate = psp.sysRadioGetSamplingRate () / 1000;
	}
	else {
		stationName = "station name";
		bitRate = 128000 / 1000;
		sampleRate = 44100 / 1000;
	}
	if ( bitRate != 0 && sampleRate != 0 ) {
		stationName = stationName
			+ " (" + bitRate + "kbps/" + sampleRate + "kHz"
			+ ((stationArray [currentStation].aacp)?"/AAC+":"") + ")";
	}
	streamTitle = unescape (stationName);
	document.getElementById ("stationInfo").firstChild.nodeValue
		= streamTitle;
}

// --------------------------------------------------------------------------
// 画面四隅のコールドスポット
// --------------------------------------------------------------------------

function hs_mouseOver () {
	if ( psp && isPSPRadio ) psp.sysRadioBackLightAlwaysOn (0);
}
function hs_mouseOut () {
	if ( psp && isPSPRadio ) psp.sysRadioBackLightAlwaysOn (1);
}

// --------------------------------------------------------------------------
// PSP 拡張プラグインオブジェクトの初期化
// --------------------------------------------------------------------------

function radioPlayerInit0 ( streamUrl ) {
	if ( bInited ) return;
	bInited = true;
	if ( psp ) {
		var result;
		if ( sw_noise_status == 1 )
			psp.sysRadioSetWhiteNoiseOscillatorVolume (60);
		if ( 0 < streamUrl.length ) {
			// とりあえず前回の最後に選局されていた放送局を鳴らし始める
			result = psp.sysRadioPlayStream
				(streamUrl, userAgentForPlayStream);
		}
		psp.sysRadioSetMasterVolume (255);
		psp.sysRadioSetSubVolume (255);
        psp.sysRadioBackLightAlwaysOn (1);
		psp.sysRadioSetDebugMode (0);
		window.resizeTo (480, 544);
		// psp.sysRadio* を実行できた場合に isPSPRadio を ture にする
		isPSPRadio = true;
        psp.sysRadioSetDebugLogTextStyle
			(224,224,224,255, 255,255,255,255, 30,30,40,96,
			 1, 0, 1);
	}
}

// --------------------------------------------------------------------------
// １日１回のティック処理
// --------------------------------------------------------------------------

function dayTickProc () {
	if ( bNowHttpGetIsBusy ) {
		return;
	}
	if ( isPSPRadio && psp ) {
		var d = new Date ();
		var url = playerTopPageUrl + "?__dt__" + startDate + "_"
			+ (1900 + d.getYear ()) + "." + (d.getMonth () + 1) + "." + d.getDate ();
		var size = 16;
		psp.sysRadioPrepareForHttpGet (url, userAgentForHttpGet, size, 0);
		// 注）これは緩い処理。排他処理は行わない。
	}
}

/*
  Local Variables:
  tab-width:4
  End:
*/
