<?php
if (!isset($_SESSION['logged']) || ($_SESSION['logged'] != 1)) {
if (session_is_registered('logged')) { session_unset(); session_destroy();
} elseif (session_is_registered('username')) { session_unset(); session_destroy();
} elseif (session_is_registered('access_level')) { session_unset(); session_destroy();
} elseif (session_is_registered('last_login')) { session_unset(); session_destroy();
} elseif (session_is_registered('last_login_ip')) { session_unset(); session_destroy();
} elseif (session_is_registered('fullname')) { session_unset(); session_destroy();
} elseif (session_is_registered('firstname')) { session_unset(); session_destroy();
} elseif (session_is_registered('middlename')) { session_unset(); session_destroy();
} elseif (session_is_registered('lastname')) { session_unset(); session_destroy();
} elseif (session_is_registered('email')) { session_unset(); session_destroy();
} elseif (session_is_registered('status')) { session_unset(); session_destroy();
} elseif (session_is_registered('mood')) { session_unset(); session_destroy();
} elseif (session_is_registered('default_image')) { session_unset(); session_destroy();
} elseif (session_is_registered('pref_song_astart')) { session_unset(); session_destroy();
} elseif (session_is_registered('pref_psong_astart')) { session_unset(); session_destroy();
} elseif (session_is_registered('pref_upstyle')) { session_unset(); session_destroy();
} elseif (session_is_registered('pref_pupstyle')) { session_unset(); session_destroy();
} elseif (session_is_registered('pref_upview')) { session_unset(); session_destroy();
} elseif (session_is_registered('setting_vmode')) { session_unset(); session_destroy();
} elseif (session_is_registered('setting_theme')) { session_unset(); session_destroy();
} elseif (session_is_registered('setting_language')) { session_unset(); session_destroy();
} elseif (session_is_registered('user_id')) { session_unset(); session_destroy();
} elseif (session_is_registered('SECURITY_CODE')) { session_unset(); session_destroy();
}
}
?>