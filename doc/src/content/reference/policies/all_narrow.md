+++
title = "All narrow (hard UB)"
weight = 10
+++
# Header file `all_narrow.hpp`

<a id="standardese-all_narrow.hpp"></a>
<pre><code class="standardese-language-cpp"><span class="pre">#include</span> <span class="pre">&quot;</span><a href="doc_common.md#standardese-common.hpp"><span class="typ dec var fun">detail&#x2F;common.hpp</span></a><span class="pre">&quot;</span>

<span class="kwd">namespace</span> <span class="typ dec var fun">outcome_v2_xxx</span>
<span class="pun">{</span>
    <span class="kwd">namespace</span> <span class="typ dec var fun">policy</span>
    <span class="pun">{</span>
        <span class="kwd">struct</span> <a href="#standardese-outcome_v2_xxx::policy::all_narrow"><span class="typ dec var fun">all_narrow</span></a><span class="pun">;</span>
    <span class="pun">}</span>
<span class="pun">}</span>
</code></pre>

<a id="standardese-outcome_v2_xxx"></a>

<a id="standardese-outcome_v2_xxx::policy"></a>

### Struct `outcome_v2_xxx::policy::all_narrow`

<a id="standardese-outcome_v2_xxx::policy::all_narrow"></a>
<pre><code class="standardese-language-cpp"><span class="kwd">struct</span> <span class="typ dec var fun">all_narrow</span>
<span class="pun">{</span>
    <span class="kwd">template</span> <span class="pun">&lt;</span><span class="kwd">class</span> <span class="typ dec var fun">Impl</span><span class="pun">&gt;</span>
    <span class="kwd">static</span> <span class="kwd">constexpr</span> <span class="kwd">void</span> <a href="#standardese-outcome_v2_xxx::policy::all_narrow::wide_value_check%3CImpl%3E(Impl&amp;&amp;)"><span class="typ dec var fun">wide_value_check</span></a><span class="pun">(</span><span class="typ dec var fun">Impl</span><span class="pun">&amp;&amp;</span> <span class="typ dec var fun">self</span><span class="pun">)</span><span class="pun">;</span>

    <span class="kwd">template</span> <span class="pun">&lt;</span><span class="kwd">class</span> <span class="typ dec var fun">Impl</span><span class="pun">&gt;</span>
    <span class="kwd">static</span> <span class="kwd">constexpr</span> <span class="kwd">void</span> <a href="#standardese-outcome_v2_xxx::policy::all_narrow::wide_error_check%3CImpl%3E(Impl&amp;&amp;)"><span class="typ dec var fun">wide_error_check</span></a><span class="pun">(</span><span class="typ dec var fun">Impl</span><span class="pun">&amp;&amp;</span> <span class="typ dec var fun">self</span><span class="pun">)</span><span class="pun">;</span>

    <span class="kwd">template</span> <span class="pun">&lt;</span><span class="kwd">class</span> <span class="typ dec var fun">Impl</span><span class="pun">&gt;</span>
    <span class="kwd">static</span> <span class="kwd">constexpr</span> <span class="kwd">void</span> <a href="#standardese-outcome_v2_xxx::policy::all_narrow::wide_exception_check%3CImpl%3E(Impl&amp;&amp;)"><span class="typ dec var fun">wide_exception_check</span></a><span class="pun">(</span><span class="typ dec var fun">Impl</span><span class="pun">&amp;&amp;</span> <span class="typ dec var fun">self</span><span class="pun">)</span><span class="pun">;</span>

    <span class="kwd">template</span> <span class="pun">&lt;</span><span class="kwd">class</span> <span class="typ dec var fun">Impl</span><span class="pun">&gt;</span>
    <span class="kwd">static</span> <span class="kwd">constexpr</span> <span class="kwd">void</span> <a href="result_exception_ptr_rethrow#standardese-outcome_v2_xxx::policy::detail::base::narrow_value_check%3CImpl%3E(Impl&amp;&amp;)"><span class="typ dec var fun">narrow_value_check</span></a><span class="pun">(</span><span class="typ dec var fun">Impl</span><span class="pun">&amp;&amp;</span> <span class="typ dec var fun">self</span><span class="pun">)</span> <span class="kwd">noexcept</span><span class="pun">;</span>

    <span class="kwd">template</span> <span class="pun">&lt;</span><span class="kwd">class</span> <span class="typ dec var fun">Impl</span><span class="pun">&gt;</span>
    <span class="kwd">static</span> <span class="kwd">constexpr</span> <span class="kwd">void</span> <a href="result_exception_ptr_rethrow#standardese-outcome_v2_xxx::policy::detail::base::narrow_error_check%3CImpl%3E(Impl&amp;&amp;)"><span class="typ dec var fun">narrow_error_check</span></a><span class="pun">(</span><span class="typ dec var fun">Impl</span><span class="pun">&amp;&amp;</span> <span class="typ dec var fun">self</span><span class="pun">)</span> <span class="kwd">noexcept</span><span class="pun">;</span>

    <span class="kwd">template</span> <span class="pun">&lt;</span><span class="kwd">class</span> <span class="typ dec var fun">Impl</span><span class="pun">&gt;</span>
    <span class="kwd">static</span> <span class="kwd">constexpr</span> <span class="kwd">void</span> <a href="result_exception_ptr_rethrow#standardese-outcome_v2_xxx::policy::detail::base::narrow_exception_check%3CImpl%3E(Impl&amp;&amp;)"><span class="typ dec var fun">narrow_exception_check</span></a><span class="pun">(</span><span class="typ dec var fun">Impl</span><span class="pun">&amp;&amp;</span> <span class="typ dec var fun">self</span><span class="pun">)</span> <span class="kwd">noexcept</span><span class="pun">;</span>
<span class="pun">};</span>
</code></pre>

Policy which treats wide checks as narrow checks.

Can be used in both `result` and `outcome`.

### Function `narrow_value_check`

<a id="standardese-outcome_v2_xxx::policy::detail::base::narrow_value_check&lt;Impl&gt;(Impl&amp;&amp;)"></a>
<pre><code class="standardese-language-cpp"><span class="kwd">template</span> <span class="pun">&lt;</span><span class="kwd">class</span> <span class="typ dec var fun">Impl</span><span class="pun">&gt;</span>
<span class="kwd">static</span> <span class="kwd">constexpr</span> <span class="kwd">void</span> <span class="typ dec var fun">narrow_value_check</span><span class="pun">(</span><span class="typ dec var fun">Impl</span><span class="pun">&amp;&amp;</span> <span class="typ dec var fun">self</span><span class="pun">)</span> <span class="kwd">noexcept</span><span class="pun">;</span>
</code></pre>

Performs a narrow check of state, used in the assume\_value() functions.

*Effects:* None.

-----

### Function `narrow_error_check`

<a id="standardese-outcome_v2_xxx::policy::detail::base::narrow_error_check&lt;Impl&gt;(Impl&amp;&amp;)"></a>
<pre><code class="standardese-language-cpp"><span class="kwd">template</span> <span class="pun">&lt;</span><span class="kwd">class</span> <span class="typ dec var fun">Impl</span><span class="pun">&gt;</span>
<span class="kwd">static</span> <span class="kwd">constexpr</span> <span class="kwd">void</span> <span class="typ dec var fun">narrow_error_check</span><span class="pun">(</span><span class="typ dec var fun">Impl</span><span class="pun">&amp;&amp;</span> <span class="typ dec var fun">self</span><span class="pun">)</span> <span class="kwd">noexcept</span><span class="pun">;</span>
</code></pre>

Performs a narrow check of state, used in the assume\_error() functions

*Effects:* None.

-----

### Function `narrow_exception_check`

<a id="standardese-outcome_v2_xxx::policy::detail::base::narrow_exception_check&lt;Impl&gt;(Impl&amp;&amp;)"></a>
<pre><code class="standardese-language-cpp"><span class="kwd">template</span> <span class="pun">&lt;</span><span class="kwd">class</span> <span class="typ dec var fun">Impl</span><span class="pun">&gt;</span>
<span class="kwd">static</span> <span class="kwd">constexpr</span> <span class="kwd">void</span> <span class="typ dec var fun">narrow_exception_check</span><span class="pun">(</span><span class="typ dec var fun">Impl</span><span class="pun">&amp;&amp;</span> <span class="typ dec var fun">self</span><span class="pun">)</span> <span class="kwd">noexcept</span><span class="pun">;</span>
</code></pre>

Performs a narrow check of state, used in the assume\_exception() functions

*Effects:* None.

-----

### Function `wide_value_check`

<a id="standardese-outcome_v2_xxx::policy::all_narrow::wide_value_check&lt;Impl&gt;(Impl&amp;&amp;)"></a>
<pre><code class="standardese-language-cpp"><span class="kwd">template</span> <span class="pun">&lt;</span><span class="kwd">class</span> <span class="typ dec var fun">Impl</span><span class="pun">&gt;</span>
<span class="kwd">static</span> <span class="kwd">constexpr</span> <span class="kwd">void</span> <span class="typ dec var fun">wide_value_check</span><span class="pun">(</span><span class="typ dec var fun">Impl</span><span class="pun">&amp;&amp;</span> <span class="typ dec var fun">self</span><span class="pun">)</span><span class="pun">;</span>
</code></pre>

Performs a wide check of state, used in the value() functions. Calls `narrow_value_check()` and does nothing else.

*Effects:* None.

-----

### Function `wide_error_check`

<a id="standardese-outcome_v2_xxx::policy::all_narrow::wide_error_check&lt;Impl&gt;(Impl&amp;&amp;)"></a>
<pre><code class="standardese-language-cpp"><span class="kwd">template</span> <span class="pun">&lt;</span><span class="kwd">class</span> <span class="typ dec var fun">Impl</span><span class="pun">&gt;</span>
<span class="kwd">static</span> <span class="kwd">constexpr</span> <span class="kwd">void</span> <span class="typ dec var fun">wide_error_check</span><span class="pun">(</span><span class="typ dec var fun">Impl</span><span class="pun">&amp;&amp;</span> <span class="typ dec var fun">self</span><span class="pun">)</span><span class="pun">;</span>
</code></pre>

Performs a wide check of state, used in the error() functions. Calls `narrow_error_check()` and does nothing else.

*Effects:* None.

-----

### Function `wide_exception_check`

<a id="standardese-outcome_v2_xxx::policy::all_narrow::wide_exception_check&lt;Impl&gt;(Impl&amp;&amp;)"></a>
<pre><code class="standardese-language-cpp"><span class="kwd">template</span> <span class="pun">&lt;</span><span class="kwd">class</span> <span class="typ dec var fun">Impl</span><span class="pun">&gt;</span>
<span class="kwd">static</span> <span class="kwd">constexpr</span> <span class="kwd">void</span> <span class="typ dec var fun">wide_exception_check</span><span class="pun">(</span><span class="typ dec var fun">Impl</span><span class="pun">&amp;&amp;</span> <span class="typ dec var fun">self</span><span class="pun">)</span><span class="pun">;</span>
</code></pre>

Performs a wide check of state, used in the exception() functions. Calls `narrow_exception_check()` and does nothing else.

*Effects:* None.

-----

-----