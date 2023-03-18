<!-- src/routes/index.svelte -->
<script>
	// const uploadServerHost = 'http://84.54.23.140:8080/';
	const uploadServerHost = 'http://localhost:8080/';

	let files = [];
	let username;
	let password;
	let loginHeader;
	let UPLOADPROGRESSVALUE = 0;

	async function getFiles() {
		const response = await fetch(uploadServerHost + 'files');
		const res = await response.json();
		files = res.fileList;
		UPLOADPROGRESSVALUE = 0;
		console.log(files);
	}

	async function handleUpload(event) {
		const selectedFile = event.target.files[0];
		event.preventDefault();
		const formData = new FormData();
		formData.append('uploaded_file', selectedFile);

		const request = new XMLHttpRequest();
		request.upload.addEventListener('progress', handleUploadProgress, false);
		request.addEventListener('load', async () => await getFiles(), false);
		request.open('POST', uploadServerHost + 'uploads');
		request.setRequestHeader('authorization', loginHeader);
		request.send(formData);
	}

	function handleUploadProgress(event) {
		UPLOADPROGRESSVALUE = Math.ceil((event.loaded / event.total) * 100);
	}

	async function deleteFromServer(filename) {
		const response = await fetch(uploadServerHost + `files/${filename}`, {
			method: 'DELETE',
			headers: { authorization: loginHeader }
		});
		await getFiles();
	}

	function loginHandler() {
		loginHeader = 'Basic ' + btoa(username + ':' + password);
	}
</script>

<div class="section">
	<div class="container is-max-desktop">
		<!-- LOGIN -->
		<h1 class="subtitle has-text-weight-bold">Login:</h1>
		<div class="field is-horizontal">
			<div class="field-body">
				<div class="field">
					<p class="control is-expanded has-icons-left">
						<input bind:value={username} class="input is-success" type="text" placeholder="Name" />
						<span class="icon is-small is-left">
							<i class="fas fa-user" />
						</span>
					</p>
				</div>
				<div class="field">
					<p class="control is-expanded has-icons-left has-icons-right">
						<input
							bind:value={password}
							class="input is-success"
							type="password"
							placeholder="Password"
						/>
						<span class="icon is-small is-left">
							<i class="fas fa-envelope" />
						</span>
						<span class="icon is-small is-right">
							<i class="fas fa-check" />
						</span>
					</p>
				</div>
			</div>

			<div class="field is-horizontal">
				<div class="field-label">
					<!-- Left empty for spacing -->
				</div>
				<div class="field-body">
					<div class="field">
						<div class="control">
							<button on:click={loginHandler} class="button is-primary"> Login </button>
						</div>
					</div>
				</div>
			</div>
		</div>

		<!-- FILE UPLOAD -->
		{#if loginHeader}
			<hr />
			<h1 class="subtitle has-text-weight-bold">File Upload:</h1>
			<div class="form-group column">
				<div class="file has-name">
					<label class="file-label">
						<input on:change={handleUpload} type="file" class="file-input" name="uploaded_file" />
						<span class="file-cta">
							<span class="file-icon">
								<i class="fas fa-upload" />
							</span>
							<span class="file-label"> Choose and upload file… </span>
						</span>
					</label>
				</div>
			</div>

			{#if UPLOADPROGRESSVALUE > 0}
				<progress class="progress is-info" value={UPLOADPROGRESSVALUE} max="100" />
			{/if}
		{/if}

		<!-- TABLE UPLOADED FILES -->
		<br />
		<hr />
		<h1 class="subtitle has-text-weight-bold">Uploaded Files:</h1>
		<table class="table is-fullwidth">
			<thead>
				<tr>
					<th>Name</th>
					<th>Media</th>
					{#if loginHeader}
						<th>Download</th>
						<th>Delete</th>
					{/if}
				</tr>
			</thead>
			<tbody>
				{#each files as file}
					<tr>
						<td>{file}</td>
						<td>
							<audio controls src="{uploadServerHost}files/{file}">
								<a class="forceFontColor" href="{uploadServerHost}files/{file}">Download</a>
							</audio>
						</td>
						{#if loginHeader}
							<td>
								<button class="button is-success">
									<a href="{uploadServerHost}files/{file}" download={file}>Download</a>
								</button>
							</td>
							<td>
								<button on:click={() => deleteFromServer(file)} class="button is-danger">
									Delete
								</button>
							</td>
						{/if}
					</tr>
				{/each}
			</tbody>
		</table>

		{#await getFiles()}
			<p>Loading...</p>
		{:then}
			{#if !files.length}
				<p>No files have been uploaded yet.</p>
			{/if}
		{/await}
	</div>
</div>

<style>
	a {
		color: white;
	}
</style>
