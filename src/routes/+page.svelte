<script>
	import { onMount } from "svelte"

	let fileInput;
	let files;
	let avatar;
	let fileList = ['loading...'];

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

	async function listFiles() {
		const lisdir = await fetch('/upload', {
			method: 'GET',
			headers: {
				'Content-Type': 'application/json',
				Accept: 'application/json'
			}
		});
		const res = await lisdir.json();
		// res = { ok: 200, dir: [...] }
		fileList = res.dir;
		return lisdir;
	}
	onMount(async () => {
		await listFiles()
	})
</script>

<!-- <h1>Welcome to SvelteKit</h1> -->
<!--<p>Visit <a href="https://kit.svelte.dev">kit.svelte.dev</a> to read the documentation</p>-->

<input
	class="hidden"
	id="file-to-upload"
	type="file"
	accept="audio/*"
	bind:files
	bind:this={fileInput}
	on:change={() => getBase64(files[0])}
/>

<!-- <a href="/calli.png">Download</a> -->
<button>Upload File</button>


<ol>
	{#each fileList as filename}
		<li>{filename}</li>
		<a href={filename}>Download {filename}</a>

		<figure>
			<figcaption>{filename}</figcaption>
			<audio
				controls
				src={filename}>
					<a href={filename}>
						Download audio
					</a>
			</audio>
		</figure>
		

	{/each}
</ol>

