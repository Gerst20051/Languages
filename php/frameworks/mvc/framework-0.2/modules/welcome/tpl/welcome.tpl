{if $user->userID > 0}
Welcome back {$user->fname}! Not {$user->fname}? Click <a href="/users/logout">here</a>!
{else}
Welcome to Framework 1.0! <a href="/users/login">Login here</a>.
{/if}
