/******************************************************************************
 * LANGUAGE
 *****************************************************************************/
/* Find browser language */
let browser_language = navigator.language || navigator.userLanguage;

/* Language in storage has higher priority */
const stored_language = localStorage.getItem('userLanguage');

/* Restore language from previous session */
if (stored_language) {
	console.log("Used language from previous session: " +
		    stored_language);
	browser_language = stored_language;
}

/* Select language from valid languages */
if (browser_language.startsWith('en')) {
	browser_language = 'en';
	console.log("User language seems to be English");
} else if (browser_language.startsWith('uk')) {
	browser_language = 'uk';
	console.log("User language seems to be Ukrainian")
} else {
	browser_language = 'en';
	console.log("User language is not recognised (defaults to english)");
}

/* Set prefered template variant (it will be the browser language) */
lw_dash_template_set_preferred_variant(browser_language);

/* Set default variant if preferred is not available */
lw_dash_template_set_default_variant('en');

/* Network dialog messages */
lw_dash_template_add_literal("LC_NET_DIALOG_TITLE",
	{en: "network", uk: "Мережа"});
lw_dash_template_add_literal("LC_NET_DIALOG_OK",
	{en: "ignore", uk: "неважливо"});
lw_dash_template_add_literal("LC_NET_DIALOG_TEXT",
	{en: "attempting to connect...",
	 uk: "спроба підключення до мережі..."});

/* Welcome message */
lw_dash_template_add_literal("LC_WELCOME_TITLE",
	{en:"welcome to CAN filter", uk:"ласкаво просимо до КАН Фільтру"});

lw_dash_template_add_literal("LC_WELCOME_TEXT",
	{en:"This software is designed to filter BMS messages, " +
	    "fix errors, and change battery parameters.\n\n" +
	    "It currently supports the following vehicles: " +
	    "Kangoo ZE, Nissan Leaf ZE0.\n\n" +

	    "For more detailed information about the interface elements, " +
	    "click the \"HINTS\" button, which will be on the " +
	    "bottom left panel.",

	 uk:"Дане програмне забезпечення призначене для " +
	    "Фільтрації повідомлень BMS, виправлення помилок " +
	    "та зміни параметрів батареї.\n\nНа даний момент існує " +
	    "Підтримка таких автомобілів: Kangoo ZE, Nissan Leaf ZE0\n\n" +
	    "Для більш детальної інформації про елементи інтерфейсу " +
	    "натисніть на кнопку \"HINTS\", яка буде знаходитись " +
	    "на нижній панелі зліва"});

lw_dash_template_add_literal("LC_UNDERSTOOD",
	{en:"understood", uk:"зрозуміло"});

const selector = lw_dash_language_selector(footer.self, [
	{name: "English",    short_name: "en", flag: "&#127468;&#127463;"},
	{name: "Українська", short_name: "uk", flag: "&#127482;&#127462;"}
	],
	function (lang) { lw_dash_template_preferred_variant = lang;
			  lw_dash_template_apply_variant(root.self, lang);
			  console.log(lang); }
);

selector.self.style.marginLeft = "auto";

/******************************************************************************
 * WIDGETS
 *****************************************************************************/
const root = lw_dash_container_h(document.body);

/* Select vehicle */
function select_vehicle(parent) {
	const overlay = lw_dash_overlay(root.self);
	const dialog  = lw_dash_dialog(overlay.self,
				       "^{LC_WELCOME_TITLE}!", ["ok"]);
	const text    = lw_dash_text(dialog.content, "^{LC_WELCOME_TEXT}");

	text.self.style.whiteSpace = "pre-wrap";
	text.self.style.fontSize   = "0.8em";

	dialog.options.ok.textContent = "^{LC_UNDERSTOOD}!";
	dialog.options.ok.onclick = function ()
	{
		overlay.remove();
		dialog.remove();
	}

	lw_dash_template_register_element(dialog.title);
	lw_dash_template_register_element(dialog.options.ok);
	lw_dash_template_register_element(text.self);
}

/* Welcome message */
function welcome_message(parent) {
	const overlay = lw_dash_overlay(root.self);
	const dialog  = lw_dash_dialog(overlay.self,
				       "^{LC_WELCOME_TITLE}!", ["ok"]);
	const text    = lw_dash_text(dialog.content, "^{LC_WELCOME_TEXT}");

	text.self.style.whiteSpace = "pre-wrap";
	text.self.style.fontSize   = "0.8em";

	dialog.options.ok.textContent = "^{LC_UNDERSTOOD}!";
	dialog.options.ok.onclick = function ()
	{
		overlay.remove();
		dialog.remove();
	}

	lw_dash_template_register_element(dialog.title);
	lw_dash_template_register_element(dialog.options.ok);
	lw_dash_template_register_element(text.self);
}

welcome_message();


/******************************************************************************
 * NETWORK
 *****************************************************************************/
const net = new lw_dash_network("ws://10.10.10.10/ws", 10000);

function net_dialog()
{
	const net_overlay = lw_dash_overlay(document.body);

	/* Create elements */
	const net_dialog  = lw_dash_dialog(net_overlay.self,
					   "^{LC_NET_DIALOG_TITLE}", ["ok"]);
	net_dialog.options.ok.textContent = "^{LC_NET_DIALOG_OK}";

	lw_dash_loader(net_dialog.content);
	const net_text = lw_dash_text(net_dialog.content,
				      "^{LC_NET_DIALOG_TEXT}");

	/* Register localization */
	lw_dash_template_register_element(net_dialog.title);
	lw_dash_template_register_element(net_dialog.options.ok);
	lw_dash_template_register_element(net_text.self);

	/* Show overlay on top of everything (as it is very important) */
	net_overlay.self.style.zIndex = 999;

	function remove() {
		net_dialog.remove();
		net_overlay.remove();
		net_text.remove();
	}

	net_dialog.options.ok.onclick = remove;

	return {
		overlay: net_overlay,
		dialog: net_dialog,
		
		remove: remove
	}
}

net_dial = net_dialog();

net.on_connect = function () {
	net_dial.remove();
};

net.on_disconnect = function () {
	net_dial = net_dialog();
};


/******************************************************************************
 * POSTPROCESSING
 *****************************************************************************/
/* Apply variant for all elements in root tree */
lw_dash_template_apply_variant(root.self, browser_language);
