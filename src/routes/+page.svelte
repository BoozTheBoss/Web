<script>
    import fs from 'fs'

	function gotFile(event) {
        let file = event.target.files[0];

        fs.writeFileSync('static/calli.png', file)
	}
</script>

<h1>Welcome to SvelteKit</h1>
<p>Visit <a href="https://kit.svelte.dev">kit.svelte.dev</a> to read the documentation</p>

<input type="file" on:input={gotFile} />
