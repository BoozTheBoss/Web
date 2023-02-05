<script>
	let fileInput;
	let files;
	let avatar;

	function getBase64(image) {
		const reader = new FileReader();
		reader.readAsDataURL(image);
		reader.onload = (e) => {
			avatar = e.target.result;
            uploadFunction(e.target.result);
		};
	}

	async function uploadFunction(imgBase64) {
		const data = {};
		const imgData = imgBase64.split(',');
		data['image'] = imgData[1];
		console.log(data);
		await fetch(`/upload`, {
			method: 'POST',
			headers: {
				'Content-Type': 'application/json',
				Accept: 'application/json'
			},
			body: JSON.stringify(data)
		});
	}
</script>

<h1>Welcome to SvelteKit</h1>
<p>Visit <a href="https://kit.svelte.dev">kit.svelte.dev</a> to read the documentation</p>

<input
	class="hidden"
	id="file-to-upload"
	type="file"
	accept=".png,.jpg"
	bind:files
	bind:this={fileInput}
	on:change={() => getBase64(files[0])}
/>
<button class="upload-btn" on:click={() => fileInput.click()}>Upload</button>

<a href="/calli.png">Download</a>
